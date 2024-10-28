#include <pybind11/pybind11.h>
#include "MyString.h"

namespace py = pybind11;

PYBIND11_MODULE(MyString, m) {
	py::class_<MyString>(m, "MyString")
		.def(py::init<char*>())
		.def(py::init())
		.def(py::init<const std::string&>())
		.def(py::init<char*, int>())
		.def(py::init<int, char>())
		.def(py::init<const MyString&>())

		.def("__add__", py::overload_cast<MyString&>(&MyString::operator+), py::is_operator(), "2")
		.def("__add__", py::overload_cast<const char*>(&MyString::operator+), py::is_operator(), "2")
		.def("__add__", py::overload_cast<std::string>(&MyString::operator+), py::is_operator(), "2")


		.def("__iadd__", py::overload_cast<const char*>(&MyString::operator+=), py::is_operator(), "2")
		.def("__iadd__", py::overload_cast<std::string>(&MyString::operator+=), py::is_operator(), "2")
		.def("__iadd__", py::overload_cast<MyString&>(&MyString::operator+=), py::is_operator(), "2")

		.def("__eq__", (&MyString::operator==), py::is_operator(), "2")

		.def("__ne__", (&MyString::operator!=), py::is_operator(), "2")

		.def("__gt__", (&MyString::operator>), py::is_operator(), "2")

		.def("__lt__", (&MyString::operator<), py::is_operator(), "2")

		.def("__ge__", (&MyString::operator>=), py::is_operator(), "2")

		.def("__le__", (&MyString::operator<=), py::is_operator(), "2")


		.def("size", &MyString::size)
		.def("c_str", &MyString::c_str)
		.def("data", &MyString::data)
		.def("length", &MyString::length)
		.def("empty", &MyString::empty)
		.def("capacity", &MyString::capacity)
		.def("shrink_to_fit", &MyString::shrink_to_fit)
		.def("clear", &MyString::clear)
		.def("at", &MyString::at)
		.def("at", &MyString::at)
		.def("to_float", &MyString::to_float)
		.def("to_int", &MyString::to_int)

		.def("insert", py::overload_cast<int, int, char>(&MyString::insert), "1")
		.def("insert", py::overload_cast<int, const char*>(&MyString::insert), "2")
		.def("insert", py::overload_cast<int, const char*, int>(&MyString::insert), "3")
		.def("insert", py::overload_cast<int, std::string>(&MyString::insert), "4")
		.def("insert", py::overload_cast<int, std::string, int>(&MyString::insert), "5")

		.def("replace", py::overload_cast<int, int, const char*>(&MyString::replace), "1")
		.def("replace", py::overload_cast<int, int, std::string>(&MyString::replace), "2")

		.def("erase", py::overload_cast<int, int>(&MyString::erase), "2")

		.def("find", py::overload_cast<const char*>(&MyString::find), "2")
		.def("find", py::overload_cast<const char*, int>(&MyString::find), "2")
		.def("find", py::overload_cast<const std::string>(&MyString::find), "2")
		.def("find", py::overload_cast<const std::string, int>(&MyString::find), "2")

		.def("substr", py::overload_cast<int, int>(&MyString::substr), "2")
		.def("substr", py::overload_cast<int>(&MyString::substr), "2") // const


		.def("append", py::overload_cast<int, char>(&MyString::append), "2")
		.def("append", py::overload_cast<const char*>(&MyString::append), "2")
		.def("append", py::overload_cast<const char*, int, int>(&MyString::append), "2")
		.def("append", py::overload_cast<const std::string>(&MyString::append), "2")
		.def("append", py::overload_cast<const std::string, int, int>(&MyString::append), "2")
		;
}
