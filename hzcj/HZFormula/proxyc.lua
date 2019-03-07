-- lua脚本执行指标运算demo
ffi = require('ffi')
string = require('string')
-- 加载指标库
formula = ffi.load('jcformula')

-- C结构定义 详情： http://luajit.org/
ffi.cdef[[ 
    typedef struct VariantMemBlock{ 
	enum {
		MT_FLOAT = 0,   // 浮点数
		MT_STRING,      // 字符串
		MT_INT64,       // int64
	} type;	
	union {
		double fdata[1]; // 浮点数
		char cdata[1]; // 4 内存碎片
		__int64 i64data[1]; // int64
	};
    int count;  // 4
    } MemBlock;
	
	/*
	 * @brief 内存分配
	 * @param count 分配数据块个数
	 */
	MemBlock* AllocDoubleMem(int count); 
	MemBlock* AllocInt64Mem(int count); 
	MemBlock* AllocStrMem(int count);
	MemBlock* CopyStr(const char* value);
	
	/*
	 * @brief 释放内存
	 * @param ptr 通过c++内存分配的数据块 通过 ffi.new("MemBlock",{}) 分配的勿使用
	 */	
    void FreeMem(MemBlock *ptr); 
	
	/*
	 * @brief 内存操作
	 */		
	int GetCount(MemBlock* ptr);
	bool IsString(MemBlock* ptr);
	bool IsNumber(MemBlock* ptr);
	double GetNumberAt(MemBlock* ptr,int index);
	const char* GetString(MemBlock* ptr);
	char GetStringAt(MemBlock* ptr,int index);
	void SetNumberAt(MemBlock* ptr,int index,double value);
	void SetStringAt(MemBlock* ptr,int index,char value);
	
	/*
	 * @brief 输出运算结果
	 * @param env 环境变量
	 * retval 输出的数据块
	 */		
	void Output(void *env, MemBlock *ptr);
	
	/*
	 * @brief 数据请求
	 * @param env 环境变量
	 * retval double类型的数据块
	 */	
	MemBlock* OPEN(void* env);
	
	/*
	 * @brief 算法实现
	 */	
	
]] 


--[[
	local autoNumBlock = ffi.gc(formula.AllocDoubleMem(100),formula.FreeMem)  --分配一段内存块，当最后一次引用时自动释放
	for i=0,formula.GetCount(autoNumBlock)-1 do
		formula.SetNumberAt(autoNumBlock,i,0.2)
	end
	formula.Output(__JC_FENV,autoNumBlock)


	local manulNumBock = ffi.new("MemBlock*",formula.AllocDoubleMem(100)) --分配一段内存块，使用完毕后需要手动释放，否则内存泄露
	for i=0,formula.GetCount(manulNumBock)-1 do
		formula.SetNumberAt(manulNumBock,i,100)  --这里设置的值无需一定要指定double，主要是根据分配内存的方式来决定的
	end
	formula.Output(__JC_FENV,manulNumBock)
	formula.FreeMem(manulBock)  -- 使用完毕后手动释放
]]

--[[
local autoStrBlock1 = ffi.gc(formula.CopyStr(ffi.new("const char*","don't give up..")),formula.FreeMem)
formula.Output(__JC_FENV,autoStrBlock1)


local autoStrBlock2 = ffi.gc(formula.AllocStrMem(100),formula.FreeMem) -- 非配固定大小的内存，并自动释放
for i=0,formula.GetCount(autoStrBlock2)-1 do                           -- 循环赋值
	formula.SetStringAt(autoStrBlock2,i,string.byte("a",1))         
	end
formula.Output(__JC_FENV,autoStrBlock2)

local manulStrBlock1 = ffi.new("MemBlock*",formula.CopyStr("create block by manual.."))
if (formula.IsString(manulStrBlock1))
then
	formula.SetStringAt(manulStrBlock1,0,string.byte("B",1))
end
formula.Output(__JC_FENV,manulStrBlock1)
formula.FreeMem(manulStrBlock1)

]]

local op = ffi.gc(formula.OPEN(__JC_FENV),formula.FreeMem)
for i = 0,formula.GetCount(op)-1 do
	formula.SetNumberAt(op,i,formula.GetNumberAt(op,i)*10)
end
formula.Output(__JC_FENV,op)


-- lua 基础教程 http://www.yiibai.com/lua/