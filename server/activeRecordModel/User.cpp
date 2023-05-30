//
// User.cpp
//
// This file has been generated from db.xml. Do not edit.
//


#include "User.h"


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
	_email(other._email),
	_created_at(other._created_at),
	_updated_at(other._updated_at)
{
}


User::Ptr User::find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id)
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(pContext->statementPlaceholderProvider());
	User::Ptr pObject(new User);

	pContext->session()
		<< "SELECT id, username, password, email, created_at, updated_at"
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
		<< "INSERT INTO users (id, username, password, email, created_at, updated_at)"
		<< "  VALUES (" << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ")",
		bind(id()),
		use(*this),
		now;
}


void User::update()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "UPDATE users"
		<< "  SET username = " << pSPP->next() << ", password = " << pSPP->next() << ", email = " << pSPP->next() << ", created_at = " << pSPP->next() << ", updated_at = " << pSPP->next()
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
		"email"s,
		"created_at"s,
		"updated_at"s,
	};

	return cols;
}


const std::string& User::table()
{
	static const std::string t = "users";
	return t;
}


} // namespace ChatRoomDB
