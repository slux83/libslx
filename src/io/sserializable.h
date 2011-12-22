/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include <cstdint>

#ifndef SSERIALIZABLE_H
#define SSERIALIZABLE_H

typedef std::int8_t   s_int8;
typedef std::int16_t  s_int16;
typedef std::int32_t  s_int32;
typedef std::int64_t  s_int64;
typedef std::uint8_t  s_uint8;
typedef std::uint16_t s_uint16;
typedef std::uint32_t s_uint32;
typedef std::uint64_t s_uint64;

/*!
	\brief Serializable base class
*/
class SSerializable
{
public:
	SSerializable();

	/*! Check if this machine has a little endian architecture
		\return true if it's little endian, false if big endian
	*/
	static bool isLittleEndian();
};

#define S_SERIALIZABLE_ID(id) \
public: \
	static s_uint32  __getSerializableId() \
	{ \
		return id; \
	}

#define S_SERIALIZABLE_INT8(name) \
public: \
	s_int8  __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_INT16(name) \
public: \
	s_int16  __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_INT32(name) \
public: \
	s_int32  __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_INT64(name) \
public: \
	s_int64  __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_UINT8(name) \
public: \
	s_uint8  __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_UINT16(name) \
public: \
	s_uint16  __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_UINT32(name) \
public: \
	s_uint32  __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_UINT64(name) \
public: \
	s_uint64  __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_FLOAT32(name) \
public: \
	float __getSerializable_##name() \
	{ \
		return name; \
	}

#define S_SERIALIZABLE_FLOAT64(name) \
public: \
	double __getSerializable_##name() \
	{ \
		return name; \
	}

//TODO FIXME
#define S_SERIALIZABLE_FIELDS(...) \
public: \
	void serialize() \
	{ \
		 \
	} \
	\
	void deserialize() {}


#endif // SSERIALIZABLE_H
