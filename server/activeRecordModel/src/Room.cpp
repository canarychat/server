//
// Room.cpp
//
// This file has been generated from db.xml. Do not edit.
//


#include "ChatRoomDB/Room.h"


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
    _owner_id(other._owner_id),
    _create_time(other._create_time),
    _update_time(other._update_time)
{
}


User::Ptr Room::owner_id() const
{
    return User::find(context(), _owner_id);
}


Room& Room::owner_id(User::Ptr pObject)
{
    if (pObject)
        _owner_id = pObject->id();
    else
        _owner_id = User::INVALID_ID;
    return *this;
}


Room::Ptr Room::find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id)
{
    Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(pContext->statementPlaceholderProvider());
    Room::Ptr pObject(new Room);

    pContext->session()
        << "SELECT id, name, owner_id, create_time, update_time"
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
        << "INSERT INTO rooms (id, name, owner_id, create_time, update_time)"
        << "  VALUES (NULL, " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ", " << pSPP->next() << ")",
        use(*this),
        now;
    updateID(context()->session());
}


void Room::update()
{
    Poco::ActiveRecord::StatementPlaceholderProvider::Ptr pSPP(context()->statementPlaceholderProvider());

    context()->session()
        << "UPDATE rooms"
        << "  SET name = " << pSPP->next() << ", owner_id = " << pSPP->next() << ", create_time = " << pSPP->next() << ", update_time = " << pSPP->next()
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
            "owner_id"s,
            "create_time"s,
            "update_time"s,
        };

    return cols;
}


const std::string& Room::table()
{
    static const std::string t = "rooms";
    return t;
}


} // namespace ChatRoomDB
