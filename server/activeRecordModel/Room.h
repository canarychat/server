//
// Room.h
//
// This file has been generated from db.xml. Do not edit.
//


#ifndef ChatRoomDB_Room_INCLUDED
#define ChatRoomDB_Room_INCLUDED


#include "Poco/ActiveRecord/ActiveRecord.h"


namespace ChatRoomDB {


class Room: public Poco::ActiveRecord::ActiveRecord<Poco::Int32>
{
public:
	using Ptr = Poco::AutoPtr<Room>;

	explicit Room(ID id);
	Room() = default;
	Room(const Room& other);
	~Room() = default;

	const std::string& name() const;
	Room& name(const std::string& value);

	const Poco::Timestamp& created_at() const;
	Room& created_at(const Poco::Timestamp& value);

	const Poco::Timestamp& updated_at() const;
	Room& updated_at(const Poco::Timestamp& value);

	static Ptr find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id);

	void insert();
	void update();
	void remove();

	static const std::vector<std::string>& columns();
	static const std::string& table();

private:
	std::string _name;
	Poco::Timestamp _created_at;
	Poco::Timestamp _updated_at;

	friend class Poco::Data::TypeHandler<Room>;
};


inline const std::string& Room::name() const
{
	return _name;
}


inline Room& Room::name(const std::string& value)
{
	_name = value;
	return *this;
}


inline const Poco::Timestamp& Room::created_at() const
{
	return _created_at;
}


inline Room& Room::created_at(const Poco::Timestamp& value)
{
	_created_at = value;
	return *this;
}


inline const Poco::Timestamp& Room::updated_at() const
{
	return _updated_at;
}


inline Room& Room::updated_at(const Poco::Timestamp& value)
{
	_updated_at = value;
	return *this;
}


} // namespace ChatRoomDB


namespace Poco {
namespace Data {


template <>
class TypeHandler<ChatRoomDB::Room>
{
public:
	static std::size_t size()
	{
		return 3;
	}

	static void bind(std::size_t pos, const ChatRoomDB::Room& ar, AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
	{
		TypeHandler<std::string>::bind(pos++, ar._name, pBinder, dir);
		TypeHandler<Poco::Timestamp>::bind(pos++, ar._created_at, pBinder, dir);
		TypeHandler<Poco::Timestamp>::bind(pos++, ar._updated_at, pBinder, dir);
}

	static void extract(std::size_t pos, ChatRoomDB::Room& ar, const ChatRoomDB::Room& deflt, AbstractExtractor::Ptr pExtr)
	{
		TypeHandler<std::string>::extract(pos++, ar._name, deflt._name, pExtr);
		TypeHandler<Poco::Timestamp>::extract(pos++, ar._created_at, deflt._created_at, pExtr);
		TypeHandler<Poco::Timestamp>::extract(pos++, ar._updated_at, deflt._updated_at, pExtr);
}

	static void prepare(std::size_t pos, const ChatRoomDB::Room& ar, AbstractPreparator::Ptr pPrep)
	{
		TypeHandler<std::string>::prepare(pos++, ar._name, pPrep);
		TypeHandler<Poco::Timestamp>::prepare(pos++, ar._created_at, pPrep);
		TypeHandler<Poco::Timestamp>::prepare(pos++, ar._updated_at, pPrep);
	}
};


} } // namespace Poco::Data


#endif // ChatRoomDB_Room_INCLUDED
