#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <Windows.h>


class CRecvProxyData;
class RecvTable;
class RecvProp;

typedef void(*NetvarHookCallback)(const CRecvProxyData *pData, void *pStruct, void *pOut);


class NetvarTree
{
	struct Node;
	using MapType = std::unordered_map<std::string, std::shared_ptr<Node>>;

	struct Node
	{
		Node(int offset) : offset(offset) {}
		MapType nodes;
		int offset;
	};

	MapType nodes;

public:
	void Hook(std::string table_to_hook, std::string prop_to_hook, NetvarHookCallback callback);
	NetvarTree();

private:
	void PopulateNodes(class RecvTable *recv_table, MapType *map);

	/**
	* GetOffsetRecursive - Return the offset of the final node
	* @map:	Node map to scan
	* @acc:	Offset accumulator
	* @name:	Netvar name to search for
	*
	* Get the offset of the last netvar from map and return the sum of it and accum
	*/
	static int GetOffsetRecursive(MapType &map, int acc, const char *name)
	{
		return acc + map[name]->offset;
	}

	/**
	* GetOffsetRecursive - Recursively grab an offset from the tree
	* @map:	Node map to scan
	* @acc:	Offset accumulator
	* @name:	Netvar name to search for
	* @args:	Remaining netvar names
	*
	* Perform tail recursion with the nodes of the specified branch of the tree passed for map
	* and the offset of that branch added to acc
	*/
	template<typename ...args_t>
	int GetOffsetRecursive(MapType &map, int acc, const char *name, args_t ...args)
	{
		const auto &node = map[name];
		return this->GetOffsetRecursive(node->nodes, acc + node->offset, args...);
	}

public:
	/**
	* GetOffset - Get the offset of a netvar given a list of branch names
	* @name:	Top level datatable name
	* @args:	Remaining netvar names
	*
	* Initiate a recursive search down the branch corresponding to the specified datable name
	*/
	template<typename ...args_t>
	int GetOffset(const char *name, args_t ...args)
	{
		const auto &node = nodes[name];
		return this->GetOffsetRecursive(node->nodes, node->offset, args...);
	}
};
extern std::unique_ptr<NetvarTree> gNetvars;

template<typename T>
class CDynamicNetvar
{
	DWORD off;

public:
	template<typename... args_t>
	CDynamicNetvar(args_t... a)
	{
		// just assume these wont be accessed until after netvars has been inited

		off = gNetvars->GetOffset(a...);
	}
	template<typename... args_t>
	CDynamicNetvar(int offset, args_t... a)
	{
		off = gNetvars->GetOffset(a...) + offset;
	}

	T GetValue(PVOID base)
	{
		return *reinterpret_cast<T *>((DWORD)base + (DWORD)off);
	}
	void SetValue(PVOID base, T val)
	{
		*reinterpret_cast<T *>((DWORD)(base)+((DWORD)(off))) = val;
	}
};

#define NETVAR(name, type, ...) static CDynamicNetvar<type> ##name( __VA_ARGS__ )
#define NETVAR_RETURN(type, base, ...) NETVAR(n, type, __VA_ARGS__); return n.GetValue(base)

#define NETVAR_OFF(name, type, offset, ...) static CDynamicNetvar<type> ##name(offset, __VA_ARGS__)
#define NETVAR_OFF_RETURN(type, base, offset, ...) NETVAR_OFF(n, type, offset, __VA_ARGS__); return n.GetValue(base)

