#ifndef _GLPT_ITERATOR
#define _GLPT_ITERATOR

#include "Library.h"

typedef std::map<std::string,std::string> EntityLoadData;

class Iterator;
class Entity;

template<typename T> static Entity* Construct(void) {return new T;}

extern Iterator* GLPT_iterator;

extern float dtof(EntityLoadData*,std::string);
extern std::string dtostr(EntityLoadData*,std::string);

class Entity {
public:
	void SetIdentity(std::string identity);
	std::string GetIdentity(void);

	void SetEventDepth(int dp);
	int GetEventDepth(void) {return depth;}

	virtual void EventCreate(EntityLoadData* data=NULL) {}
	virtual void EventStep(void) {}
	virtual void EventDraw(void) {}
	virtual void EventTrigger(int,void*) {}
	void Destroy(void);

	void PostWarning(std::string message);
private:

	friend class Iterator;

	Iterator* iid;
	unsigned int entity_id;
	int depth;
	bool managed;
	std::string identity;
};

class Iterator {
public:
	static void Create(void) {
		GLPT_iterator=new Iterator;
	}
	static void Release(void) {
		delete GLPT_iterator;
	}
	void BroadcastStep(void);
	void BroadcastDraw(void);
	void BroadcastTrigger(int,void*);
	void BindGlobalMap(Entity*,std::string);
	void OnDepth(void);
	void Remove(Entity*);
	Entity* GetByIdentity(std::string identity);
	void Push(Entity*,std::string="",EntityLoadData* data=NULL);
	template <typename T> Entity* Push(std::string predef_identity="",EntityLoadData* data=NULL) {
		// Templated function, must be inlined.

		Entity* new_entity=reinterpret_cast<Entity*>(new T);

		new_entity->iid=this;
		new_entity->entity_id=entity_id_index++;

		if (predef_identity!="") {
			new_entity->SetIdentity(predef_identity);
		}

		entity_buffer.push_back(new_entity);

		new_entity->EventCreate(data);

		return new_entity;
	}
	void Load(std::string filename,bool append);
	template <typename T> void BindEntityName(std::string name) {
		entity_type_index[name]=&Construct<T>;
	}
	~Iterator(void);
private:
	void Sort(void);
	Iterator(void) {
		entity_id_index=0;
	}
	std::vector<Entity*> entity_buffer;
	std::map<std::string,Entity*> identity_index;
	unsigned int entity_id_index;

	// Entity loading module.

	std::map<std::string,Entity*(*)(void)> entity_type_index;
};

#endif