// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
#include <signal.h>
#include <string.h>
#include <boost/pool/singleton_pool.hpp>
#include "NetMsg.h"
#include "CryptMem.h"
#include "SEED_256_KISA.h"
#include "LCCRC32.h"
#include "logsystem.h"

CLCCRC32 gCRC32;

struct tag_pool {};
typedef boost::singleton_pool<tag_pool, (MAX_MESSAGE_SIZE + sizeof(int))> g_netmsgpool;

//////////////////////////////////////////////////////////////////////
// Inicializacion para el cifrado SEED
static uint32_t pdwRoundKey[48];
static BYTE pbUserKey[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
							 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
							};

void CNetMsg::InitSEEDEncrypt()
{
	SeedRoundKey(pdwRoundKey, pbUserKey);
	// ### Debug
	/*
	LOG_INFO( "CNetMsg::InitSEEDEncrypt");
	FILE *file = fopen("pdwRoundKey.dat", "wb");
	fwrite((void *)pdwRoundKey, sizeof(char), sizeof(pdwRoundKey), file);
	fclose(file);
	*/
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNetMsg::CNetMsg()
	:m_mtype(MSG_UNKNOWN)
	,m_buf_all(NULL)
	,m_buf(NULL)
{
	Init();
}

CNetMsg::CNetMsg(const CNetMsg& src)
{
	m_buf_all = m_buf = NULL;
	Init(src);
}

CNetMsg::CNetMsg(const CNetMsg::SP& src)
{
	m_buf_all = m_buf = NULL;
	Init(src);
}

CNetMsg::CNetMsg(int mtype)
{
	m_buf_all = m_buf = NULL;
	Init(mtype);
}

CNetMsg::CNetMsg(int mtype, int key)
{
	m_buf_all = m_buf = NULL;
	Init(mtype, key);
}

CNetMsg::~CNetMsg()
{
	if (m_buf_all)
	{
		g_netmsgpool::free(m_buf_all);
	}
}

void CNetMsg::Init()
{
	m_mtype = MSG_UNKNOWN;
	if (!m_buf_all)
	{
		m_buf_all = (unsigned char*)g_netmsgpool::malloc();
		m_buf = m_buf_all + sizeof(MsgHeader);
	}
	m_size = 0;
	m_ptr = 0;

	secretkey = 0;

	crypt_flag_ = false;
	crc32_flag_ = false;
	make_header_flag_ = false;

	// check sum
	m_buf_all[MAX_MESSAGE_SIZE + 0] = 0xCC;
	m_buf_all[MAX_MESSAGE_SIZE + 1] = 0xCC;
	m_buf_all[MAX_MESSAGE_SIZE + 2] = 0xCC;
	m_buf_all[MAX_MESSAGE_SIZE + 3] = 0xCC;
}

void CNetMsg::Init(int mtype)
{
	Init();
	m_mtype = mtype;

	unsigned char ubtype = (unsigned char)m_mtype;
	*this << ubtype;

	secretkey = 0;
}

void CNetMsg::Init(const CNetMsg& src)
{
	Init();
	m_mtype = src.m_mtype;
	m_size = src.m_size;
	memcpy(m_buf, src.m_buf, m_size);
	m_ptr = src.m_ptr;

	secretkey = src.secretkey;
}

void CNetMsg::Init(const CNetMsg::SP& src)
{
	Init();
	m_mtype = src->m_mtype;
	m_size = src->m_size;
	memcpy(m_buf, src->m_buf, m_size);
	m_ptr = src->m_ptr;

	secretkey = src->secretkey;
}

void CNetMsg::Init(int mtype, int key)
{
	Init();
	m_mtype = mtype;

	unsigned char ubtype = (unsigned char)m_mtype;
	*this << ubtype;

	secretkey = key;
}

void CNetMsg::Read(void* buf, long size)
{
	if (size < 1 || !CanRead(size))
		return ;
	memcpy(buf, m_buf + m_ptr, size);
	m_ptr += size;
}

void CNetMsg::Write(void* buf, long size)
{
	if (size < 1 || !CanWrite(size))
		return ;
	memcpy(m_buf + m_ptr, buf, size);
	m_ptr += size;
	if (m_ptr > m_size)
		m_size = m_ptr;
}

void CNetMsg::WriteRaw(void* buf, long size)
{
	if (size > MAX_MESSAGE_SIZE)
	{
		Init(MSG_UNKNOWN);
		return ;
	}
	unsigned char* mtype = (unsigned char*)buf;
	Init((int)(*mtype), secretkey);
	Write(mtype + MAX_MESSAGE_TYPE, size - MAX_MESSAGE_TYPE);
	m_size = size;
	m_ptr = 0;
	MoveFirst();
}

CNetMsg& CNetMsg::operator >> (CLCString& str)
{
	// Busca el terminador NULL desde la posicion actual
	int i;
	for (i = m_ptr; i < m_size; i++)
	{
		if (m_buf[i] == '\0')
			break;
	}

	// Si encuentra NULL
	if (i < m_size)
	{
		// Calcula la longitud, incluido NULL
		int len = i - m_ptr + 1;
		// Calcula la longitud que se leera
		int realRead = len;
		// Limita la longitud al tamano del buffer de salida
		if (realRead > str.BufferSize())
			realRead = str.BufferSize();
		// Lee los datos
		char* tmp = new char[realRead];
		memcpy(tmp, m_buf + m_ptr, realRead - 1);
		tmp[realRead - 1] = '\0';
		str = tmp;
		delete [] tmp;
		// Avanza m_ptr despues de NULL aunque se haya truncado la lectura
		m_ptr += len;
	}
	// Si no lo encuentra
	else
	{
		// No hay datos para leer
		// Invalida la siguiente posicion de lectura y completa la operacion
		str = "";
		m_ptr = m_size;
	}

	return *this;
}

CNetMsg& CNetMsg::operator << (const char* str)
{
	if (str == NULL)
		return *this;

	// Escribe la cadena con NULL si cabe; si no, solo NULL; si tampoco cabe, se detiene
	if (CanWrite(strlen(str) + 1))
	{
		Write((void*)((const char*)str), strlen(str) + 1);
	}
	else if (CanWrite())
	{
		char ch = 0;
		*this << ch;
	}
	return *this;
}

CNetMsg& CNetMsg::operator << (CNetMsg& src)
{
	if (src.m_size < 1 || !CanWrite(src.m_size))
		return *this;
	memcpy(m_buf + m_ptr, src.m_buf, src.m_size);
	m_ptr += src.m_size;
	if (m_ptr > m_size)
		m_size = m_ptr;

	return *this;
}

CNetMsg& CNetMsg::operator << (CNetMsg::SP& src)
{
	if (src->m_size < 1 || !CanWrite(src->m_size))
		return *this;
	memcpy(m_buf + m_ptr, src->m_buf, src->m_size);
	m_ptr += src->m_size;
	if (m_ptr > m_size)
		m_size = m_ptr;

	return *this;
}

void CNetMsg::crypt()
{
	int loopcount = this->m_size / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedEncrypt(this->m_buf + (i * 16), pdwRoundKey);
	}

	crypt_flag_ = true;
}

void CNetMsg::decrypt()
{
	int loopcount = this->m_size / 16;
	for (int i = 0; i < loopcount; ++i)
	{
		SeedDecrypt(this->m_buf + (i * 16), pdwRoundKey);
	}
}

bool CNetMsg::checkCRC32()
{
	extern CLCCRC32 gCRC32;

	unsigned int nRecvCRCResult = *(unsigned int *)(this->m_buf + this->m_size);
	HTONL(nRecvCRCResult);

	unsigned int nCRCResult = gCRC32.CalcCRC32(this->m_buf, this->m_size);

	return (nRecvCRCResult == nCRCResult);
}

void CNetMsg::makeCRC32()
{
	// only wirte
	extern CLCCRC32 gCRC32;

	unsigned int nCRCResult = gCRC32.CalcCRC32(this->m_buf, this->m_size);
	HTONL(nCRCResult);
	*(unsigned int *)(this->m_buf + this->m_size) = nCRCResult;

	crc32_flag_ = true;
}

void CNetMsg::makeHeader()
{
	MsgHeader* h = (MsgHeader*)m_buf_all;

	h->reliable = (1 << 0) | (1 << 7) | (1 << 8);
	HTONS(h->reliable);

	// rnsocketioserviceTCP genera seq
	h->id = 0;
	h->size = static_cast<uint32_t>(m_size);
	HTONL(h->size);

	make_header_flag_ = true;
}

// Reduce el paquete en length bytes desde m_buf + pos.
void CNetMsg::Reduce( int pos, int length )
{
	::memmove(m_buf + pos, m_buf + pos + length, m_size - length);
	m_size -= length;
}

void CNetMsg::setSize( int size )
{
	m_mtype = m_buf[0];
	m_size = size;

	if (m_size >= (MAX_MESSAGE_SIZE - sizeof(MsgHeader)))
		raise(SIGABRT);
}

bool CNetMsg::isRightPacket()
{
	if (m_buf_all[MAX_MESSAGE_SIZE + 0] != 0xCC
		|| m_buf_all[MAX_MESSAGE_SIZE + 1] != 0xCC
		|| m_buf_all[MAX_MESSAGE_SIZE + 2] != 0xCC
		|| m_buf_all[MAX_MESSAGE_SIZE + 3] != 0xCC)
		return false;

	return true;
}
