#ifndef JSON_H
#define JSON_H

#include "nlohmann.hpp"
using Json = nlohmann::json;

#define jtov_enum3(var, jnode, key) \
do { \
	Json::iterator it = jnode->find(key); \
	if (it != jnode->end()) (var) = static_cast<decltype(var)>(static_cast<int>(*it)); \
}while(0)
#define jtov_enum2(var, key) jtov_enum3(var, jnode, key)
#define jtov_enum(var) jtov_enum2(var, #var)


#define jtov3(var, jnode, key) \
do { \
	Json::iterator it = jnode->find(key); \
	if (it != jnode->end()) (var) = *it; \
}while(0)
#define jtov2(var, key) jtov3(var, jnode, key)
#define jtov(var) jtov2(var, #var)

#define vtoj3(var, jnode, key) jnode[key] = var
#define vtoj2(var, key) jnode[key] = var
#define vtoj(var) jnode[#var] = var

#define With(Type) do { struct Visitor : Type { void operator()() {
#define End(var) }}; static_cast<Visitor&>((var))(); } while(0);

#define WithJP(Type) do { struct Visitor : Type { void operator()(Json* jnode) {
#define WithJR(Type) do { struct Visitor : Type { void operator()(Json& jnode) {
#define EndJ(var) }}; static_cast<Visitor&>((var))(jnode); } while(0);

#endif // JSON_H
