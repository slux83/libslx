/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SVARIANT_H
#define SVARIANT_H

#include <typeinfo>
#include <string>
#include "../global/sglobal.h"

/*!
	\namespace internal stuff
*/
namespace internalS
{
	/*!
		\brief Abstract variant
	*/
	class SAbstractVariant
	{
	public:
		virtual ~SAbstractVariant() {}
	};

	/*!
		\brief Typed variant implementation
	*/
	template <typename T>
	class SVariantImpl : public SAbstractVariant
	{
	public:
		T data;

		SVariantImpl(T value) : data(value) {}

		virtual ~SVariantImpl() {}
	};
}

/*!
	\brief Generic Variant type
*/
class SVariant
{
private:
	internalS::SAbstractVariant *data;

public:
	template <typename VariantType>
	explicit SVariant(VariantType value)
	{
		data =  new internalS::SVariantImpl<VariantType>(value);
	}

	explicit SVariant() : data(NULL) {}

	virtual ~SVariant()
	{
		if (data != NULL)
			delete data;

		data = NULL;
	}

	/*!
		Get row data copy
		\return the \b copy of internal data or an application assert
		\sa isTypeOf()
	*/
	template <typename VariantType>
	operator VariantType () const
	{
		S_ASSERT_MSG(data != NULL, "SVariant: NULL internal data");

		//We try to cast the internal data variant with VariantType
		internalS::SVariantImpl<VariantType> *base = dynamic_cast< internalS::SVariantImpl<VariantType>* >(data);

		//Fail?
		S_ASSERT_MSG(base != NULL, "SVariant: incompatible type cast.");

		return base->data;
	}

	/*!
		Use this function to test if this variant can be stored in a variable of the same type of \b tester
		\return true on success, false otherwise. Return false if the internal data is NULL
	*/
	template <typename VariantType>
	bool isTypeOf()
	{
		if (data == NULL)
			return false;

		return typeid(*data) == typeid(internalS::SVariantImpl<VariantType>);
	}

};

#endif // SVARIANT_H
