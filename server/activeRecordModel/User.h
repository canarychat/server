//
// User.h
//
// This file has been generated from db.xml. Do not edit.
//


#ifndef ChatRoomDB_User_INCLUDED
#define ChatRoomDB_User_INCLUDED


#include "Poco/ActiveRecord/ActiveRecord.h"


namespace ChatRoomDB {


class User: public Poco::ActiveRecord::ActiveRecord<Poco::Int32>
{
public:
	using Ptr = Poco::AutoPtr<User>;

	explicit User(ID id);
	User() = default;
	User(const User& other);
	~User() = default;

	const std::string& username() const;
	User& username(const std::string& value);

	const std::string& password() const;
	User& password(const std::string& value);

	const std::string& email() const;
	User& email(const std::string& value);

	const Poco::Timestamp& created_at() const;
	User& created_at(const Poco::Timestamp& value);

	const Poco::Timestamp& updated_at() const;
	User& updated_at(const Poco::Timestamp& value);

	static Ptr find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id);

	void insert();
	void update();
	void remove();

	static const std::vector<std::string>& columns();
	static const std::string& table();

private:
	std::string _username;
	std::string _password;
	std::string _email;
	Poco::Timestamp _created_at;
	Poco::Timestamp _updated_at;

	friend class Poco::Data::TypeHandler<User>;
};


inline const std::string& User::username() const
{
	return _username;
}


inline User& User::username(const std::string& value)
{
	_username = value;
	return *this;
}


inline const std::string& User::password() const
{
	return _password;
}


inline User& User::password(const std::string& value)
{
	_password = value;
	return *this;
}


inline const std::string& User::email() const
{
	return _email;
}


inline User& User::email(const std::string& value)
{
	_email = value;
	return *this;
}


inline const Poco::Timestamp& User::created_at() const
{
	return _created_at;
}


inline User& User::created_at(const Poco::Timestamp& value)
{
	_created_at = value;
	return *this;
}


inline const Poco::Timestamp& User::updated_at() const
{
	return _updated_at;
}


inline User& User::updated_at(const Poco::Timestamp& value)
{
	_updated_at = value;
	return *this;
}


} // namespace ChatRoomDB


namespace Poco {
namespace Data {


template <>
class TypeHandler<ChatRoomDB::User>
{
public:
	static std::size_t size()
	{
		return 5;
	}

	static void bind(std::size_t pos, const ChatRoomDB::User& ar, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		TypeHandler<std::string>::bind(pos++, ar._username, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, ar._password, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, ar._email, pBinder, dir);
		TypeHandler<Poco::Timestamp>::bind(pos++, ar._created_at, pBinder, dir);
		TypeHandler<Poco::Timestamp>::bind(pos++, ar._updated_at, pBinder, dir);
}

	static void extract(std::size_t pos, ChatRoomDB::User& ar, const ChatRoomDB::User& deflt, AbstractExtractor::Ptr pExtr)
	{
		TypeHandler<std::string>::extract(pos++, ar._username, deflt._username, pExtr);
		TypeHandler<std::string>::extract(pos++, ar._password, deflt._password, pExtr);
		TypeHandler<std::string>::extract(pos++, ar._email, deflt._email, pExtr);
		TypeHandler<Poco::Timestamp>::extract(pos++, ar._created_at, deflt._created_at, pExtr);
		TypeHandler<Poco::Timestamp>::extract(pos++, ar._updated_at, deflt._updated_at, pExtr);
}

	static void prepare(std::size_t pos, const ChatRoomDB::User& ar, AbstractPreparator::Ptr pPrep)
	{
		TypeHandler<std::string>::prepare(pos++, ar._username, pPrep);
		TypeHandler<std::string>::prepare(pos++, ar._password, pPrep);
		TypeHandler<std::string>::prepare(pos++, ar._email, pPrep);
		TypeHandler<Poco::Timestamp>::prepare(pos++, ar._created_at, pPrep);
		TypeHandler<Poco::Timestamp>::prepare(pos++, ar._updated_at, pPrep);
	}
};


} } // namespace Poco::Data


#endif // ChatRoomDB_User_INCLUDED
