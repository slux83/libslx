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
		//! Distructor
		virtual ~SAbstractVariant() {}

		/*! Clone function
			\return a new instance of SAbstractVariant starting from this instance's data
		*/
		virtual SAbstractVariant* clone() = 0;
	};

	/*!
		\brief Typed variant implementation
	*/
	template <typename T>
	class SVariantImpl : public SAbstractVariant
	{
	public:
		//! Raw data
		T data;

		//! Constructor
		SVariantImpl(T value) : data(value) {}

		//! Distructor
		virtual ~SVariantImpl() {}

	protected:
		/*! Clone function
			\return a new instance of SAbstractVariant starting from this instance's data
		*/
		virtual SAbstractVariant* clone()
		{
			return new SVariantImpl<T>(data);
		}
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
	/*!
		Constructor
		\param the Variant value (any type)
		\example SVariant variant(100); SVariant variant(std::string("hello")); SVariant variant(3.14);
	*/
	template <typename VariantType>
	explicit SVariant(VariantType value)
	{
		data =  new internalS::SVariantImpl<VariantType>(value);
	}

	//! Constructor
	explicit SVariant() : data(NULL) {}

	//! Copy Constructor
	SVariant(const SVariant &right)
	{
		data = right.data->clone();
	}

	//! Distructor
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
		Use this function to test if this variant can be stored in a variable of a particular type
		\return true on success, false otherwise. Return false if the internal data is NULL
		\example variant.isTypeOf<int>();
	*/
	template <typename VariantType>
	bool isTypeOf()
	{
		if (data == NULL)
			return false;

		return typeid(*data) == typeid(internalS::SVariantImpl<VariantType>);
	}

	/*! Assignment operator
		\param right SVariant
	*/
	SVariant& operator= (const SVariant &right)
	{
		if (this == &right)
			return *this;

		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		data = right.data->clone();
		return *this;
	}

};

#endif // SVARIANT_H
