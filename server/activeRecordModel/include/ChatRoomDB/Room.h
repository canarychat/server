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

	const Poco::Data::Date& create_time() const;
	Room& create_time(const Poco::Data::Date& value);

	const Poco::Data::Date& update_time() const;
	Room& update_time(const Poco::Data::Date& value);

	static Ptr find(Poco::ActiveRecord::Context::Ptr pContext, const ID& id);

	void insert();
	void update();
	void remove();

	static const std::vector<std::string>& columns();
	static const std::string& table();

private:
	std::string _name;
	Poco::Data::Date _create_time;
	Poco::Data::Date _update_time;

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


inline const Poco::Data::Date& Room::create_time() const
{
	return _create_time;
}


inline Room& Room::create_time(const Poco::Data::Date& value)
{
	_create_time = value;
	return *this;
}


inline const Poco::Data::Date& Room::update_time() const
{
	return _update_time;
}


inline Room& Room::update_time(const Poco::Data::Date& value)
{
	_update_time = value;
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
		TypeHandler<Poco::Data::Date>::bind(pos++, ar._create_time, pBinder, dir);
		TypeHandler<Poco::Data::Date>::bind(pos++, ar._update_time, pBinder, dir);
}

	static void extract(std::size_t pos, ChatRoomDB::Room& ar, const ChatRoomDB::Room& deflt, AbstractExtractor::Ptr pExtr)
	{
		TypeHandler<std::string>::extract(pos++, ar._name, deflt._name, pExtr);
		TypeHandler<Poco::Data::Date>::extract(pos++, ar._create_time, deflt._create_time, pExtr);
		TypeHandler<Poco::Data::Date>::extract(pos++, ar._update_time, deflt._update_time, pExtr);
}

	static void prepare(std::size_t pos, const ChatRoomDB::Room& ar, AbstractPreparator::Ptr pPrep)
	{
		TypeHandler<std::string>::prepare(pos++, ar._name, pPrep);
		TypeHandler<Poco::Data::Date>::prepare(pos++, ar._create_time, pPrep);
		TypeHandler<Poco::Data::Date>::prepare(pos++, ar._update_time, pPrep);
	}
};


} } // namespace Poco::Data


#endif // ChatRoomDB_Room_INCLUDED