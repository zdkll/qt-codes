#pragma once
#include <memory>
#include <assert.h>
#include <string>
// 无效数据
#define INVALID_MEMBLOCKDATA_FLOAT    ((double)DBL_MAX)
#define INVALID_MEMBLOCKDATA_STRING   ((char)'\0')
#define INVALID_MEMBLOCKDATA_INT64    ((__int64)0xffffffffffffffffL)

struct  VariantMemBlock {

private:
	/**
	 * @breif 数据类型
	 */
	enum {
		MT_FLOAT	= 0x1,		// 浮点数
		MT_STRING	= 0x2,		// 字符串
		MT_INT64	= 0x4,		// int64
		MT_SINGLE   = 0x8,      // 单值
	} type;

	/**
	 * @breif 大小， 为-1表示为单值 (4)
	 */
	int count;

	/**
	 * @breif 数据块 (8)
	 */
	union {
		/*下面的做法可以确保字段不会为NULL，免去判断的麻烦，而且避免了内存碎片*/
		double fdata[1]; // 浮点数
		char cdata[1]; // 4 内存碎片
		__int64 i64data[1]; // int64
	};
protected:
	template <typename T>
	static VariantMemBlock* AllocMem(int count, T value)
	{
		// 保证内存块大小为int类型大小(4)的整数倍
		// 这样可以减少内存碎片
		VariantMemBlock *block = NULL;
		if (count > 0) {
			int bitsize = sizeof(VariantMemBlock) + sizeof(T)*(count - 8 / sizeof(T));
			int bitmode = bitsize%sizeof(int);
			if (bitmode != 0) {
				bitsize += (sizeof(int) - bitmode);
			}
			assert(bitsize%sizeof(int) == 0);
			block = (VariantMemBlock*)malloc(bitsize);
			assert(NULL != block);
			block->count = count;
		}
		else if (count >= -1){
			// count < -1 应该报错
			assert(count >= -1);
			block = (VariantMemBlock*)malloc(sizeof(VariantMemBlock));
			block->count = (-1 == count ? -1 : 1);
		}
		return block;
	}

public:
	// 默认内存池
	//static jc_formula::IMemoryPool* memorypool_;
	/**
	* @brief 释放内存块
	*/
	static void Free(VariantMemBlock *block)
	{
		if (NULL != block) {
			//JC_INFO_LOG(win_out_dbg, "Free(%d)", block->GetCount());
			free((void*)block);
		}
	}

	/**
	* @brief 申请MemBlock内存块
	* @param[in] count 内存块中数据格式个数， 为-1代表单值数据
    * @param[in] value 初始数据，也通过这个来确定MemBLock中数据的类型
    */
    static VariantMemBlock* Alloc(int count, double value)
    {
		VariantMemBlock *block = AllocMem(count, value);
		if (block)
		{
			block->type = MT_FLOAT;
			// 赋初始值
              //memset(block->fdata,static_cast<double>(value),block->GetCount()* sizeof(double));
            for (int i = block->GetCount() - 1; i >= 0; --i) {
                block->fdata[i] = value;
            }
		}
		return block;
	}

	static VariantMemBlock* AllocFloat(int count)
	{
        VariantMemBlock *block = AllocMem(count,0.0);
		if (block)
		{
			block->type = MT_FLOAT;
            memset(block->fdata,0,block->GetCount()* sizeof(double));
		}
		return block;
	}

	static VariantMemBlock* Alloc(int count, __int64 value)
	{
		VariantMemBlock *block = AllocMem(count, value);
		if (block)
		{
			block->type = MT_INT64;
			// 赋初始值
            memset(block->i64data,static_cast<int>(value),block->GetCount()*sizeof(double));
//            for (int i = block->GetCount() - 1; i >= 0; --i) {
//                block->i64data[i] = value;
//            }
		}
		return block;
	}

	static VariantMemBlock* AllocStr(unsigned int count, const char *value)
	{
		if (NULL == value) {
			value = "";
		}
		count = count > strlen(value) ? count : strlen(value) + 1;
		VariantMemBlock *block = AllocMem(count, value);
		if (block)
		{
			block->type = MT_STRING;
			block->count = count;
			// 赋初始值
			memcpy(block->cdata, value, block->count);
		}
		return block;
	}

	static VariantMemBlock* Alloc(const char *value)
	{
		if (NULL == value) {
			value = "";
		}
		int count = strlen(value) + 1;
		VariantMemBlock *block = AllocMem(count, value);
		if (block)
		{
			block->type = MT_STRING;
			block->count = count;
			// 赋初始值
			memcpy(block->cdata, value, block->count);
		}
		return block;
	}

	/**
	* @brief 赋值
	*
	*/
	static inline bool Assign(VariantMemBlock *desblock, int desstart,
								const VariantMemBlock *sorblock, int sorstart,
								int count) 
	{
		if (NULL == desblock || NULL == sorblock
			|| desblock->type != sorblock->type
			|| desstart < 0 || sorstart < 0
			|| count <= 0
			|| desstart > desblock->GetCount()
			|| count - sorstart > desblock->GetCount()
			|| sorstart + count > sorblock->GetCount()) {
			assert(false);
			return false;
		}
		switch (desblock->type) {
		case MT_FLOAT: {
			memcpy(desblock->fdata + desstart,
				sorblock->fdata + sorstart,
				sizeof(double)*count);
		}
					   break;
		case MT_STRING: {
			memcpy(desblock->cdata + desstart,
				sorblock->cdata + sorstart,
				sizeof(double)*count);
		}
						break;
		case MT_INT64: {
			memcpy(desblock->i64data + desstart,
				sorblock->i64data + sorstart,
				sizeof(double)*count);
		}
					   break;
		default:
			break;
		}
		return true;
	}

	static VariantMemBlock* Resize(VariantMemBlock *block, int size)
	{
		VariantMemBlock *newblock = nullptr;
		if (block)
		{
			switch (block->type) {
			case MT_FLOAT: {
				newblock = VariantMemBlock::Alloc(size, INVALID_MEMBLOCKDATA_FLOAT);
			}
						   break;
			case MT_STRING: {
				newblock = VariantMemBlock::AllocStr(size, INVALID_MEMBLOCKDATA_STRING);
			}
							break;
			case MT_INT64: {
				newblock = VariantMemBlock::Alloc(size, INVALID_MEMBLOCKDATA_INT64);
			}
						   break;
			default:
				break;
			}
			int end = size > block->GetCount() ? block->GetCount() : size;
			VariantMemBlock::Assign(newblock, 0, block, 0, end);
		}
		VariantMemBlock::Free(block);
		block = newblock;
		return newblock;
	}

    //填充当前内容
    static bool fillBlock(VariantMemBlock *block, int pos,char* data,int len)
    {
        bool bRet = false;
        if(block && data && len > 0 && pos >= 0)
        {
            int count = block->GetCount();
            int size = (count - pos);
            if(size > 0 )
            {
                switch (block->type) {
                    case MT_FLOAT:
                    {
                        int num = sizeof(double)*size;
                        num = num>len?len:num;
                        memcpy(block->fdata,data,num);
                        bRet = true;
                    }
                    break;
                case MT_STRING:
                    {
                        int num = size>len?len:size;
                        memcpy(block->fdata,block->cdata,num);
                        bRet =true;
                    }
                    break;
                case MT_INT64:
                    {
                         int num = sizeof(time_t)*size;
                         num = num>len?len:num;
                         memcpy(block->i64data,data,num);
                         bRet = true;
                    }
                 break;
                default:
                    break;
                }
            }
        }
        return bRet;
    }


	/**
	* @brief 截取，当start为0，count为data.count时表示为克隆
	*
	*/
	static VariantMemBlock* SubBlock(const VariantMemBlock *block,
		                             int start,
		                             int count)
	{
		if (NULL == block
			|| start < 0
			|| count <= 0
			|| start + count > block->GetCount()) {
			assert(false);
			return NULL;
		}
		VariantMemBlock *newblock = NULL;
		switch (block->type) {
		case MT_FLOAT: {
			int bitsize = sizeof(VariantMemBlock) + sizeof(double)*(count - 1);
			assert(bitsize%sizeof(int) == 0);
			//newblock = (VariantMemBlock*)new char[bitsize];
			newblock = (VariantMemBlock*)malloc(bitsize);
			if (NULL != newblock) {
				newblock->type = MT_FLOAT;
				newblock->count = (block->IsSingleVal() ? -1 : count);
				memcpy(newblock->fdata,
					block->fdata + start,
					sizeof(double)*count);
			}
		}
					   break;
		case MT_STRING: {
			int bitsize = sizeof(VariantMemBlock) + count - 8;
			int bitmode = bitsize%sizeof(int);
			if (bitmode != 0) {
				bitsize += (sizeof(int) - bitmode);
			}
			assert(bitsize%sizeof(int) == 0);
			//newblock = (VariantMemBlock*)new char[bitsize];
			newblock = (VariantMemBlock*)malloc(bitsize);
			if (NULL != newblock) {
				newblock->type = MT_STRING;
				newblock->count = (block->IsSingleVal() ? -1 : count);
				memcpy(newblock->cdata,
					block->cdata + start,
					count);
			}
		}
						break;
		case MT_INT64: {
			int bitsize = sizeof(VariantMemBlock) + sizeof(__int64)*(count - 1);
			assert(bitsize%sizeof(int) == 0);
			//newblock = (VariantMemBlock*)new char[bitsize];
			newblock = (VariantMemBlock*)malloc(bitsize);
			if (NULL != newblock) {
				newblock->type = MT_INT64;
				newblock->count = (block->IsSingleVal() ? -1 : count);
				memcpy(newblock->i64data,
					block->i64data + start,
					sizeof(time_t)*count);
			}
		}
					   break;
		default:
			break;
		}
		assert(NULL != newblock);
		return newblock;
	}

	/**
	* @brief 判断是否为单值数据
	*/
	inline bool IsSingleVal() const {
		return -1 == count;
	}

	/**
	* @brief 获取数据长度
	*/
	inline int GetCount() const {
		return -1 == count ? 1 : count;
	}

	/**
	* @brief 判断数据类型
	*/
	inline bool IsNumber() const {
		return ((MT_FLOAT | MT_INT64) & type) == 1 ? true : false;
	}
	inline bool IsFloat() const {
		return (MT_FLOAT & type) == 1 ? true : false;
	}
	inline bool IsString() const {
		return (MT_STRING & type) == 1 ? true : false;
	}
	inline bool IsInt64() const {
		return (MT_INT64 & type) == 1 ? true : false;
	}

	/**
	* @brief 获取数据
	* @param[in] pos 数据位置
	* @return 获取到的数据
	*
	* 使用前先保证数据类型正确
	*/
	inline double GetNumberAt(int pos) const {

		//if (IsSingleVal()) {
		//	pos = 0;
		//}
		switch (type)
		{
		case VariantMemBlock::MT_FLOAT:
			return fdata[pos];
			break;
		case VariantMemBlock::MT_INT64:
			return (double)i64data[pos];
			break;
		default:
			break;
		}
		////assert(pos >= 0 && pos < GetCount() && IsNumber());
		//if (IsFloat()) {
		//	return fdata[pos];
		//}
		//else if (IsInt64()) {
		//	return (double)i64data[pos];
		//}
		return INVALID_MEMBLOCKDATA_FLOAT;
	}

	inline double GetFloatAt(int pos) const {
		if (IsSingleVal()) {
			pos = 0;
		}
		assert(pos >= 0 && pos < GetCount() && IsFloat());
		return fdata[pos];
	}

	inline char GetStringAt(int pos) const {
		if (IsSingleVal()) {
			pos = 0;
		}
		assert(pos >= 0 && pos < GetCount() && IsString());
		return cdata[pos];
	}

	inline __int64 GetInt64At(int pos) const {
		if (IsInt64()) {
			pos = 0;
		}
		assert(pos >= 0 && pos < GetCount() && IsInt64());
		return i64data[pos];
	}

	/**
	* @brief 设置数据
	* @param[in] pos 数据位置
	* @param[in] value 新值
	*
	* 使用前先保证数据类型正确
	*/
	inline void SetNumberAt(int pos, double value) {
		//if (IsSingleVal()) {
		//	pos = 0;
		//}
		//assert(pos >= 0 && pos < GetCount() && IsNumber());
		switch (type) {
		case MT_FLOAT:
			fdata[pos] = value;
			break;
		case MT_INT64:
			i64data[pos] = (__int64)value;
			break;
		}
	}

	inline void SetFloatAt(int pos, double value) {
		if (IsSingleVal()) {
			pos = 0;
		}
		assert(pos >= 0 && pos < GetCount() && IsFloat());
		if (IsFloat()) {
			fdata[pos] = value;
		}
	}

	inline void SetStringAt(int pos, char value) {
		if (IsSingleVal()) {
			pos = 0;
		}
		assert(pos >= 0 && pos < GetCount() && IsString());
		if (IsString()) {
			cdata[pos] = value;
		}
	}

	inline void SetInt64At(int pos, __int64 value) {
		if (IsSingleVal()) {
			pos = 0;
		}
		assert(pos >= 0 && pos < GetCount() && IsInt64());
		if (IsInt64()) {
			i64data[pos] = value;
		}
	}

	/**
	* @brief 获取/设置字符串数据
	* @param[in] pos 数据位置
	* @param[in] value 新值
	*
	* 使用前先保证数据类型正确
	*/
	inline std::string GetString() const {
		assert(IsString());
		return std::string(cdata, GetCount());
	}
	inline void setString(const char *value) {
		assert(IsString());
		if (NULL == value) {
			value = "";
		}
		/*delete[]cdata;*/
		count = strlen(value) + 1;
		// 赋初始值
		memcpy(cdata, value, count + 1);
	}

	/**
	* @brief 判断数据是否有效
	*/
	inline bool IsValid(int i) const {
		if (IsSingleVal()) {
			i = 0;
		}
		assert(i >= 0 && i < GetCount());
		switch (type) {
		case MT_FLOAT:
			return INVALID_MEMBLOCKDATA_FLOAT != fdata[i];
		case MT_STRING:
			return INVALID_MEMBLOCKDATA_STRING != cdata[i];
		case MT_INT64:
			return INVALID_MEMBLOCKDATA_INT64 != i64data[i];
		}
		return false;
	}

	/**
	* @brief 获取首个有效数据的位置
	* @return 返回基于0索引的位置
	*/
	int GetFirstValidPosition() const {
		int pos = 0;
		if (!IsSingleVal()) {
			switch (type) {
			case MT_FLOAT:
				while (pos < count && INVALID_MEMBLOCKDATA_FLOAT == fdata[pos]) {
					++pos;
				}
				break;
			case MT_STRING:
				while (pos < count && INVALID_MEMBLOCKDATA_STRING == cdata[pos]) {
					++pos;
				}
				break;
			case MT_INT64:
				while (pos < count && INVALID_MEMBLOCKDATA_INT64 == i64data[pos]) {
					++pos;
				}
				break;
			default:
				pos = INT_MAX;
				break;
			}
		}
		return pos;
	}

};

typedef VariantMemBlock MemBlock;
