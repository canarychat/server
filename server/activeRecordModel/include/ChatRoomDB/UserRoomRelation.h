//
// UserRoomRelation.h
//
// This file has been generated from db.xml. Do not edit.
//


#ifndef ChatRoomDB_UserRoomRelation_INCLUDED
#define ChatRoomDB_UserRoomRelation_INCLUDED


#include "Poco/ActiveRecord/ActiveRecord.h"
#include "ChatRoomDB/User.h"
#include "ChatRoomDB/Room.h"


namespace ChatRoomDB {


class UserRoomRelation: public Poco::ActiveRecord::KeylessActiveRecord
{
public:
	using Ptr = Poco::AutoPtr<UserRoomRelation>;

	UserRoomRelation() = default;
	UserRoomRelation(const UserRoomRelation& other);
	~UserRoomRelation() = default;

	const Poco::Data::Date& joined_at() const;
	UserRoomRelation& joined_at(const Poco::Data::Date& value);

	User::Ptr user_id() const;
	Poco::Int32 user_idID() const;
	UserRoomRelation& user_id(User::Ptr pObject);
	UserRoomRelation& user_idID(Poco::Int32 id);

	Room::Ptr room_id() const;
	Poco::Int32 room_idID() const;
	UserRoomRelation& room_id(Room::Ptr pObject);
	UserRoomRelation& room_idID(Poco::Int32 id);

	void insert();
	void update();
	void remove();

	static const std::vector<std::string>& columns();
	static const std::string& table();

private:
	Poco::Int32 _user_id = User::INVALID_ID;
	Poco::Int32 _room_id = Room::INVALID_ID;
	Poco::Data::Date _joined_at;

	friend class Poco::Data::TypeHandler<UserRoomRelation>;
};


inline const Poco::Data::Date& UserRoomRelation::joined_at() const
{
	return _joined_at;
}


inline UserRoomRelation& UserRoomRelation::joined_at(const Poco::Data::Date& value)
{
	_joined_at = value;
	return *this;
}


inline Poco::Int32 UserRoomRelation::user_idID() const
{
	return _user_id;
}


inline UserRoomRelation& UserRoomRelation::user_idID(Poco::Int32 value)
{
	_user_id = value;
	return *this;
}


inline Poco::Int32 UserRoomRelation::room_idID() const
{
	return _room_id;
}


inline UserRoomRelation& UserRoomRelation::room_idID(Poco::Int32 value)
{
	_room_id = value;
	return *this;
}


} // namespace ChatRoomDB


namespace Poco {
namespace Data {


template <>
class TypeHandler<ChatRoomDB::UserRoomRelation>
{
public:
	static std::size_t size()
	{
		return 3;
	}

	static void bind(std::size_t pos, const ChatRoomDB::UserRoomRelation& ar, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		TypeHandler<Poco::Int32>::bind(pos++, ar._user_id, pBinder, dir);
		TypeHandler<Poco::Int32>::bind(pos++, ar._room_id, pBinder, dir);
		TypeHandler<Poco::Data::Date>::bind(pos++, ar._joined_at, pBinder, dir);
}

	static void extract(std::size_t pos, ChatRoomDB::UserRoomRelation& ar, const ChatRoomDB::UserRoomRelation& deflt, AbstractExtractor::Ptr pExtr)
	{
		TypeHandler<Poco::Int32>::extract(pos++, ar._user_id, deflt._user_id, pExtr);
		TypeHandler<Poco::Int32>::extract(pos++, ar._room_id, deflt._room_id, pExtr);
		TypeHandler<Poco::Data::Date>::extract(pos++, ar._joined_at, deflt._joined_at, pExtr);
}

	static void prepare(std::size_t pos, const ChatRoomDB::UserRoomRelation& ar, AbstractPreparator::Ptr pPrep)
	{
		TypeHandler<Poco::Int32>::prepare(pos++, ar._user_id, pPrep);
		TypeHandler<Poco::Int32>::prepare(pos++, ar._room_id, pPrep);
		TypeHandler<Poco::Data::Date>::prepare(pos++, ar._joined_at, pPrep);
	}
};


} } // namespace Poco::Data


#endif // ChatRoomDB_UserRoomRelation_INCLUDED
