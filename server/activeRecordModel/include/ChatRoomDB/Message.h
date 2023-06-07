//
// Message.h
//
// This file has been generated from db.xml. Do not edit.
//


#ifndef ChatRoomDB_Message_INCLUDED
#define ChatRoomDB_Message_INCLUDED


#include "Poco/ActiveRecord/ActiveRecord.h"
#include "ChatRoomDB/User.h"
#include "ChatRoomDB/Room.h"


namespace ChatRoomDB {


class Message: public Poco::ActiveRecord::ActiveRecord<Poco::Int64>
{
  public:
    using Ptr = Poco::AutoPtr<Message>;

    explicit Message(ID id);
    Message() = default;
    Message(const Message& other);
    ~Message() = default;

    const std::string& type() const;
    Message& type(const std::string& value);

    const std::string& content() const;
    Message& content(const std::string& value);

    Poco::Int64 timestamp() const;
    Message& timestamp(Poco::Int64 value);

    User::Ptr sender_id() const;
    Poco::Int32 sender_idID() const;
    Message& sender_id(User::Ptr pObject);
    Message& sender_idID(Poco::Int32 id);

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
    Poco::Int32 _sender_id = User::INVALID_ID;
    Poco::Int32 _room_id = Room::INVALID_ID;
    std::string _type;
    std::string _content;
    Poco::Int64 _timestamp = 0;

    friend class Poco::Data::TypeHandler<Message>;
};


inline const std::string& Message::type() const
{
    return _type;
}


inline Message& Message::type(const std::string& value)
{
    _type = value;
    return *this;
}


inline const std::string& Message::content() const
{
    return _content;
}


inline Message& Message::content(const std::string& value)
{
    _content = value;
    return *this;
}


inline Poco::Int64 Message::timestamp() const
{
    return _timestamp;
}


inline Message& Message::timestamp(Poco::Int64 value)
{
    _timestamp = value;
    return *this;
}


inline Poco::Int32 Message::sender_idID() const
{
    return _sender_id;
}


inline Message& Message::sender_idID(Poco::Int32 value)
{
    _sender_id = value;
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
        return 5;
    }

    static void bind(std::size_t pos, const ChatRoomDB::Message& ar, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
    {
        TypeHandler<Poco::Int32>::bind(pos++, ar._sender_id, pBinder, dir);
        TypeHandler<Poco::Int32>::bind(pos++, ar._room_id, pBinder, dir);
        TypeHandler<std::string>::bind(pos++, ar._type, pBinder, dir);
        TypeHandler<std::string>::bind(pos++, ar._content, pBinder, dir);
        TypeHandler<Poco::Int64>::bind(pos++, ar._timestamp, pBinder, dir);
    }

    static void extract(std::size_t pos, ChatRoomDB::Message& ar, const ChatRoomDB::Message& deflt, AbstractExtractor::Ptr pExtr)
    {
        TypeHandler<Poco::Int32>::extract(pos++, ar._sender_id, deflt._sender_id, pExtr);
        TypeHandler<Poco::Int32>::extract(pos++, ar._room_id, deflt._room_id, pExtr);
        TypeHandler<std::string>::extract(pos++, ar._type, deflt._type, pExtr);
        TypeHandler<std::string>::extract(pos++, ar._content, deflt._content, pExtr);
        TypeHandler<Poco::Int64>::extract(pos++, ar._timestamp, deflt._timestamp, pExtr);
    }

    static void prepare(std::size_t pos, const ChatRoomDB::Message& ar, AbstractPreparator::Ptr pPrep)
    {
        TypeHandler<Poco::Int32>::prepare(pos++, ar._sender_id, pPrep);
        TypeHandler<Poco::Int32>::prepare(pos++, ar._room_id, pPrep);
        TypeHandler<std::string>::prepare(pos++, ar._type, pPrep);
        TypeHandler<std::string>::prepare(pos++, ar._content, pPrep);
        TypeHandler<Poco::Int64>::prepare(pos++, ar._timestamp, pPrep);
    }
};


} } // namespace Poco::Data


#endif // ChatRoomDB_Message_INCLUDED
