/** 
 * @file buffer.h
 * @brief 缓存块管理
 * @author zhangzewu 
 * @date 16/07/2013
 *
 * 详细说明   
*/

#pragma once

#include <memory>

	template <class type>
	class _buffer_base
	{
	public:
		//大小
		int size() { return m_nBufLen; }
		//读取内存块
		type* getbuffer() { return m_pBuffer; }

		void resize(int nBufLen)
		{
			if(m_nBufLen != nBufLen)
			{
				type *pBuffer = new type[nBufLen];
				if(m_pBuffer)
				{
					memcpy_s((void*)pBuffer, nBufLen * sizeof(type), (void*)m_pBuffer, m_nBufLen * sizeof(type));
					delete [] m_pBuffer;
				}
				m_pBuffer = pBuffer;
				m_nBufLen = nBufLen;
			}
		}

	protected:
		_buffer_base(int nBufLen):m_nBufLen(nBufLen) { m_pBuffer = new type[nBufLen]; }
		virtual~_buffer_base() { delete[]m_pBuffer; }

		type *m_pBuffer;
		int m_nBufLen;
	};

	template <class type = unsigned char>
	class _buffer : public _buffer_base<type>
	{
	public:
		typedef std::shared_ptr<_buffer> buffer_ptr;

		static buffer_ptr CreateFactory(int nBufLen)
		{
			return buffer_ptr(new _buffer(nBufLen));
		}
		//拷贝
		buffer_ptr copy()
		{
			buffer_ptr copyone = CreateFactory(m_nBufLen);
			memcpy_s((void*)copyone->getbuffer(), copyone->size() * sizeof(type), (void*)m_pBuffer, m_nBufLen * sizeof(type));
			return copyone;
		}
	protected:
		_buffer(int nBufLen):_buffer_base<type>(nBufLen){}
	};
	typedef _buffer<> buffer;
	typedef buffer::buffer_ptr buffer_ptr;


