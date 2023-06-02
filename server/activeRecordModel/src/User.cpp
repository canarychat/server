//
// User.cpp
//
// This file has been generated from db.xml. Do not edit.
//


#include "ChatRoomDB/User.h"


using namespace std::string_literals;
using namespace Poco::Data::Keywords;


namespace ChatRoomDB {


User::User(ID id):
	Poco::ActiveRecord::ActiveRecord<Poco::Int32>(id)
{
}


User::User(const User& other):
	Poco::ActiveRecord::ActiveRecord<Poco::Int32>(other),
	_username(other._username),
	_password(other._password),
	_salt(other._salt),
	_email(other._email),
	_create_time(other._create_time),
	_update_time(other._update_time)
{
}


User::Ptr User::find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id)
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(pContext->statementPlaceholderProvider());
	User::Ptr pObject(new User);

	pContext->session()
		<< "SELECT id, username, password, salt, email, create_time, update_time"
		<< "  FROM users"
		<< "  WHERE id = " << pSPP->next(),
		into(pObject->mutableID()),
		into(*pObject),
		bind(id),
		now;

	return withContext(pObject, pContext);
}


void User::insert()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "INSERT INTO users (id, username, password, salt, email, create_time, update_time)"
		<< "  VALUES (NULL, " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ")",
		use(*this),
		now;
	updateID(context()->session());
}


void User::update()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "UPDATE users"
		<< "  SET username = " << pSPP->next() << ", password = " << pSPP->next() << ", salt = " << pSPP->next() << ", email = " << pSPP->next() << ", create_time = " << pSPP->next() << ", update_time = " << pSPP->next()
		<< "  WHERE id = " << pSPP->next(),
		use(*this),
		bind(id()),
		now;
}


void User::remove()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "DELETE FROM users"
		<< "  WHERE id = " << pSPP->next(),
		bind(id()),
		now;
}


const std::vector<std::string>& User::columns()
{
	static const std::vector<std::string> cols =
	{
		"id"s,
		"username"s,
		"password"s,
		"salt"s,
		"email"s,
		"create_time"s,
		"update_time"s,
	};

	return cols;
}


const std::string& User::table()
{
	static const std::string t = "users";
	return t;
}


} // namespace ChatRoomDB
