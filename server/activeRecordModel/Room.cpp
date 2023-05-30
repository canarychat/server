//
// Room.cpp
//
// This file has been generated from db.xml. Do not edit.
//


#include "Room.h"


using namespace std::string_literals;
using namespace Poco::Data::Keywords;


namespace ChatRoomDB {


Room::Room(ID id):
	Poco::ActiveRecord::ActiveRecord<Poco::Int32>(id)
{
}


Room::Room(const Room& other):
	Poco::ActiveRecord::ActiveRecord<Poco::Int32>(other),
	_name(other._name),
	_created_at(other._created_at),
	_updated_at(other._updated_at)
{
}


Room::Ptr Room::find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id)
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(pContext->statementPlaceholderProvider());
	Room::Ptr pObject(new Room);

	pContext->session()
		<< "SELECT id, name, created_at, updated_at"
		<< "  FROM rooms"
		<< "  WHERE id = " << pSPP->next(),
		into(pObject->mutableID()),
		into(*pObject),
		bind(id),
		now;

	return withContext(pObject, pContext);
}


void Room::insert()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "INSERT INTO rooms (id, name, created_at, updated_at)"
		<< "  VALUES (" << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ")",
		bind(id()),
		use(*this),
		now;
}


void Room::update()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "UPDATE rooms"
		<< "  SET name = " << pSPP->next() << ", created_at = " << pSPP->next() << ", updated_at = " << pSPP->next()
		<< "  WHERE id = " << pSPP->next(),
		use(*this),
		bind(id()),
		now;
}


void Room::remove()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "DELETE FROM rooms"
		<< "  WHERE id = " << pSPP->next(),
		bind(id()),
		now;
}


const std::vector<std::string>& Room::columns()
{
	static const std::vector<std::string> cols =
	{
		"id"s,
		"name"s,
		"created_at"s,
		"updated_at"s,
	};

	return cols;
}


const std::string& Room::table()
{
	static const std::string t = "rooms";
	return t;
}


} // namespace ChatRoomDB
