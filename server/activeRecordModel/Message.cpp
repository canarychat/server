//
// Message.cpp
//
// This file has been generated from db.xml. Do not edit.
//


#include "Message.h"


using namespace std::string_literals;
using namespace Poco::Data::Keywords;


namespace ChatRoomDB {


Message::Message(ID id):
	Poco::ActiveRecord::ActiveRecord<Poco::Int32>(id)
{
}


Message::Message(const Message& other):
	Poco::ActiveRecord::ActiveRecord<Poco::Int32>(other),
	_user_id(other._user_id),
	_room_id(other._room_id),
	_message(other._message),
	_created_at(other._created_at)
{
}


User::Ptr Message::user_id() const
{
	return User::find(context(), _user_id);
}


Message& Message::user_id(User::Ptr pObject)
{
	if (pObject)
		_user_id = pObject->id();
	else
		_user_id = User::INVALID_ID;
	return *this;
}


Room::Ptr Message::room_id() const
{
	return Room::find(context(), _room_id);
}


Message& Message::room_id(Room::Ptr pObject)
{
	if (pObject)
		_room_id = pObject->id();
	else
		_room_id = Room::INVALID_ID;
	return *this;
}


Message::Ptr Message::find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id)
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(pContext->statementPlaceholderProvider());
	Message::Ptr pObject(new Message);

	pContext->session()
		<< "SELECT id, user_id, room_id, message, created_at"
		<< "  FROM messages"
		<< "  WHERE id = " << pSPP->next(),
		into(pObject->mutableID()),
		into(*pObject),
		bind(id),
		now;

	return withContext(pObject, pContext);
}


void Message::insert()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "INSERT INTO messages (id, user_id, room_id, message, created_at)"
		<< "  VALUES (" << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ")",
		bind(id()),
		use(*this),
		now;
}


void Message::update()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "UPDATE messages"
		<< "  SET user_id = " << pSPP->next() << ", room_id = " << pSPP->next() << ", message = " << pSPP->next() << ", created_at = " << pSPP->next()
		<< "  WHERE id = " << pSPP->next(),
		use(*this),
		bind(id()),
		now;
}


void Message::remove()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "DELETE FROM messages"
		<< "  WHERE id = " << pSPP->next(),
		bind(id()),
		now;
}


const std::vector<std::string>& Message::columns()
{
	static const std::vector<std::string> cols =
	{
		"id"s,
		"user_id"s,
		"room_id"s,
		"message"s,
		"created_at"s,
	};

	return cols;
}


const std::string& Message::table()
{
	static const std::string t = "messages";
	return t;
}


} // namespace ChatRoomDB
