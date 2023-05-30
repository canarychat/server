//
// Message.h
//
// This file has been generated from db.xml. Do not edit.
//


#ifndef ChatRoomDB_Message_INCLUDED
#define ChatRoomDB_Message_INCLUDED


#include "Poco/ActiveRecord/ActiveRecord.h"
#include "User.h"
#include "Room.h"


namespace ChatRoomDB {


class Message: public Poco::ActiveRecord::ActiveRecord<Poco::Int32>
{
public:
	using Ptr = Poco::AutoPtr<Message>;

	explicit Message(ID id);
	Message() = default;
	Message(const Message& other);
	~Message() = default;

	const std::string& message() const;
	Message& message(const std::string& value);

	const Poco::Timestamp& created_at() const;
	Message& created_at(const Poco::Timestamp& value);

	User::Ptr user_id() const;
	Poco::Int32 user_idID() const;
	Message& user_id(User::Ptr pObject);
	Message& user_idID(Poco::Int32 id);

	Room::Ptr room_id() const;
	Poco::Int32 room_idID() const;
	Message& room_id(Room::Ptr pObject);
	Message& room_idID(Poco::Int32 id);

	static Ptr find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id);

	void insert();
	void update();
	void remove();

	static const std::vector<std::string>& columns();
	static const std::string& table();

private:
	Poco::Int32 _user_id = User::INVALID_ID;
	Poco::Int32 _room_id = Room::INVALID_ID;
	std::string _message;
	Poco::Timestamp _created_at;

	friend class Poco::Data::TypeHandler<Message>;
};


inline const std::string& Message::message() const
{
	return _message;
}


inline Message& Message::message(const std::string& value)
{
	_message = value;
	return *this;
}


inline const Poco::Timestamp& Message::created_at() const
{
	return _created_at;
}


inline Message& Message::created_at(const Poco::Timestamp& value)
{
	_created_at = value;
	return *this;
}


inline Poco::Int32 Message::user_idID() const
{
	return _user_id;
}


inline Message& Message::user_idID(Poco::Int32 value)
{
	_user_id = value;
	return *this;
}


inline Poco::Int32 Message::room_idID() const
{
	return _room_id;
}


inline Message& Message::room_idID(Poco::Int32 value)
{
	_room_id = value;
	return *this;
}


} // namespace ChatRoomDB


namespace Poco {
namespace Data {


template <>
class TypeHandler<ChatRoomDB::Message>
{
public:
	static std::size_t size()
	{
		return 4;
	}

	static void bind(std::size_t pos, const ChatRoomDB::Message& ar, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		TypeHandler<Poco::Int32>::bind(pos++, ar._user_id, pBinder, dir);
		TypeHandler<Poco::Int32>::bind(pos++, ar._room_id, pBinder, dir);
		TypeHandler<std::string>::bind(pos++, ar._message, pBinder, dir);
		TypeHandler<Poco::Timestamp>::bind(pos++, ar._created_at, pBinder, dir);
}

	static void extract(std::size_t pos, ChatRoomDB::Message& ar, const ChatRoomDB::Message& deflt, AbstractExtractor::Ptr pExtr)
	{
		TypeHandler<Poco::Int32>::extract(pos++, ar._user_id, deflt._user_id, pExtr);
		TypeHandler<Poco::Int32>::extract(pos++, ar._room_id, deflt._room_id, pExtr);
		TypeHandler<std::string>::extract(pos++, ar._message, deflt._message, pExtr);
		TypeHandler<Poco::Timestamp>::extract(pos++, ar._created_at, deflt._created_at, pExtr);
}

	static void prepare(std::size_t pos, const ChatRoomDB::Message& ar, AbstractPreparator::Ptr pPrep)
	{
		TypeHandler<Poco::Int32>::prepare(pos++, ar._user_id, pPrep);
		TypeHandler<Poco::Int32>::prepare(pos++, ar._room_id, pPrep);
		TypeHandler<std::string>::prepare(pos++, ar._message, pPrep);
		TypeHandler<Poco::Timestamp>::prepare(pos++, ar._created_at, pPrep);
	}
};


} } // namespace Poco::Data


#endif // ChatRoomDB_Message_INCLUDED