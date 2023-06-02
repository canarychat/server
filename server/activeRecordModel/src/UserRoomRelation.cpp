//
// UserRoomRelation.cpp
//
// This file has been generated from db.xml. Do not edit.
//


#include "ChatRoomDB/UserRoomRelation.h"


using namespace std::string_literals;
using namespace Poco::Data::Keywords;


namespace ChatRoomDB {


UserRoomRelation::UserRoomRelation(const UserRoomRelation& other):
	_user_id(other._user_id),
	_room_id(other._room_id),
	_joined_at(other._joined_at)
{
}


User::Ptr UserRoomRelation::user_id() const
{
	return User::find(context(), _user_id);
}


UserRoomRelation& UserRoomRelation::user_id(User::Ptr pObject)
{
	if (pObject)
		_user_id = pObject->id();
	else
		_user_id = User::INVALID_ID;
	return *this;
}


Room::Ptr UserRoomRelation::room_id() const
{
	return Room::find(context(), _room_id);
}


UserRoomRelation& UserRoomRelation::room_id(Room::Ptr pObject)
{
	if (pObject)
		_room_id = pObject->id();
	else
		_room_id = Room::INVALID_ID;
	return *this;
}


void UserRoomRelation::insert()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "INSERT INTO user_room_relation (user_id, room_id, joined_at)"
		<< "  VALUES (" << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ")",
		use(*this),
		now;
}


void UserRoomRelation::update()
{
	throw Poco::NotImplementedException("update not implemented for keyless class", "UserRoomRelation");
}


void UserRoomRelation::remove()
{
	Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

	context()->session()
		<< "DELETE FROM user_room_relation"
		<< "  WHERE user_id = " << pSPP->next() << " AND room_id = " << pSPP->next() << " AND joined_at = " << pSPP->next(),
		use(*this),
		now;
}


const std::vector<std::string>& UserRoomRelation::columns()
{
	static const std::vector<std::string> cols =
	{
		"user_id"s,
		"room_id"s,
		"joined_at"s,
	};

	return cols;
}


const std::string& UserRoomRelation::table()
{
	static const std::string t = "user_room_relation";
	return t;
}


} // namespace ChatRoomDB
