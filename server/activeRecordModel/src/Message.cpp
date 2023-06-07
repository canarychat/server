//
// Message.cpp
//
// This file has been generated from db.xml. Do not edit.
//


#include "ChatRoomDB/Message.h"


using namespace std::string_literals;
using namespace Poco::Data::Keywords;


namespace ChatRoomDB {


Message::Message(ID id):
    Poco::ActiveRecord::ActiveRecord<Poco::Int64>(id)
{
}


Message::Message(const Message& other):
    Poco::ActiveRecord::ActiveRecord<Poco::Int64>(other),
    _sender_id(other._sender_id),
    _room_id(other._room_id),
    _type(other._type),
    _content(other._content),
    _timestamp(other._timestamp)
{
}


User::Ptr Message::sender_id() const
{
    return User::find(context(), _sender_id);
}


Message& Message::sender_id(User::Ptr pObject)
{
    if (pObject)
        _sender_id = pObject->id();
    else
        _sender_id = User::INVALID_ID;
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
        << "SELECT id, sender_id, room_id, type, content, timestamp"
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
        << "INSERT INTO messages (id, sender_id, room_id, type, content, timestamp)"
        << "  VALUES (NULL, " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ")",
        use(*this),
        now;
    updateID(context()->session());
}


void Message::update()
{
    Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

    context()->session()
        << "UPDATE messages"
        << "  SET sender_id = " << pSPP->next() << ", room_id = " << pSPP->next() << ", type = " << pSPP->next() << ", content = " << pSPP->next() << ", timestamp = " << pSPP->next()
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
            "sender_id"s,
            "room_id"s,
            "type"s,
            "content"s,
            "timestamp"s,
        };

    return cols;
}


const std::string& Message::table()
{
    static const std::string t = "messages";
    return t;
}


} // namespace ChatRoomDB
