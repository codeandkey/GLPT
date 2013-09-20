#include "Iterator.h"

float dtof(EntityLoadData* dat,std::string index) {
	if (!dat) {
		GLPT_logger.Print("[GLPT_entity] EntityLoadData : dtof called with NULL data!");
		return 0.0f;
	}

	try {
		(*dat).at(index);
	} catch (std::out_of_range) {
		GLPT_logger.Print("[GLPT_entity] EntityLoadData : looking for value " + index + ", not found in data!");
		return 0.0f;
	}

	return atof((*dat)[index].c_str());
}

std::string dtostr(EntityLoadData* dat,std::string index) {
	if (!dat) {
		GLPT_logger.Print("[GLPT_entity] EntityLoadData : dtof called with NULL data!");
		return "";
	}

	try {
		(*dat).at(index);
	} catch (std::out_of_range) {
		GLPT_logger.Print("[GLPT_entity] EntityLoadData : looking for value " + index + ", not found in data!");
		return "";
	}

	return (*dat)[index];
}

Iterator* GLPT_iterator=NULL;

void Iterator::BroadcastDraw(void) {
	for(unsigned int i=0;i<entity_buffer.size();i++) {
		if (entity_buffer[i]) {
			entity_buffer[i]->EventDraw();
		}
	}
}

void Iterator::BroadcastStep(void) {
	for(unsigned int i=0;i<entity_buffer.size();i++) {
		if (entity_buffer[i]) {
			entity_buffer[i]->EventStep();
		}
	}
}

void Iterator::BroadcastTrigger(int id,void* param) {
	for(unsigned int i=0;i<entity_buffer.size();i++) {
		if (entity_buffer[i]) {
			entity_buffer[i]->EventTrigger(id,param);
		}
	}
}

void Iterator::Push(Entity* new_entity,std::string predef_identity,EntityLoadData* data) {

	new_entity->iid=this;
	new_entity->entity_id=entity_id_index++;
	
	if (predef_identity!="") {
		new_entity->SetIdentity(predef_identity);
	}

	new_entity->managed=false;
	new_entity->depth=0;

	entity_buffer.push_back(new_entity);

	new_entity->EventCreate(data);

}

void Iterator::BindGlobalMap(Entity* target,std::string identity) {

	try {
		identity_index.at(identity);
	} catch(std::out_of_range) {
		identity_index[identity]=target;
		return;
	}

	identity_index[identity]->identity="[overwritten]";
	identity_index[identity]=target;
}

Entity* Iterator::GetByIdentity(std::string identity) {

	try {
		identity_index.at(identity);
	} catch(std::out_of_range) {
		return NULL;
	}

	return identity_index[identity];

}

void Iterator::OnDepth(void) {
	Sort();
}

void Iterator::Sort(void) {

	// First, sort out all NULL values.

	for(unsigned int i=0;i<entity_buffer.size();i++) {

		if (!entity_buffer[i]) {
			entity_buffer.erase(entity_buffer.begin() + i);
			i--; // Go back to avoid out-of-bounds.
		}

	}

	for(unsigned int i=0;i<entity_buffer.size();i++) {

		int high_depth=entity_buffer[i]->GetEventDepth();
		int high_id=i;

		for(unsigned int f=i;f<entity_buffer.size();f++) {
			if (entity_buffer[f]->GetEventDepth() > high_depth) {
				high_id=f;
				high_depth=entity_buffer[f]->GetEventDepth();
			}
		}

		// Swap I and F.
		if (i!=high_id) {
			Entity* temp=entity_buffer[i];
			entity_buffer[i]=entity_buffer[high_id];
			entity_buffer[high_id]=temp;
		}

	}

}

void Iterator::Remove(Entity* target) {

	for(unsigned int i=0;i<entity_buffer.size();i++) {
		if (entity_buffer[i]==target) {
			entity_buffer.erase(entity_buffer.begin() + i);
		}
	}

}

Iterator::~Iterator(void) {
	while(entity_buffer.size()) {
		if (entity_buffer[0]) {
			if (entity_buffer[0]->managed) delete entity_buffer[0];
		}
		entity_buffer.erase(entity_buffer.begin());
	}

	entity_buffer.clear();
}

void Entity::SetIdentity(std::string identity) {
	this->identity=identity;

	iid->BindGlobalMap(this,identity);
}

void Entity::SetEventDepth(int dp) {
	depth=dp;
	iid->OnDepth();
}

std::string Entity::GetIdentity(void) {
	return identity;
}

void Entity::PostWarning(std::string message) {
	GLPT_logger.Print("[GLPT_entity] Entity posted fault [" + ((identity!="") ? identity : "anonymous") + "] : " + message);
}

void Entity::Destroy(void) {
	this->EventDestroy();
	iid->Remove(this);
}

// Iterator loading modules.

void Iterator::Load(std::string filename,bool append) {

	filename="Map/"+filename+".cdm";

	GLPT_logger.Print("[GLPT_iterator] Loading map " + filename);

	if (append) this->~Iterator(); // Wipe the current entity buffer.

	std::ifstream file;
	file.open(filename.c_str(),std::ios::in | std::ios::binary);

	if (!file) {
		GLPT_logger.Print("[GLPT_iterator] Cannot find map file " +filename);
		return;
	}

	unsigned int entity_count;
	
	file.read(reinterpret_cast<char*>(&entity_count),sizeof(unsigned int));

	for(unsigned int i=0;i<entity_count;i++) {

		std::string type,identity;
		unsigned int type_size,identity_size,key_count,key_size,value_size;
		std::string key,value;
		EntityLoadData pass_data;
		Entity* new_entity;

		file.read(reinterpret_cast<char*>(&type_size),sizeof(unsigned int));
		type.resize(type_size);
		file.read(&type[0],type_size);
		file.read(reinterpret_cast<char*>(&identity_size),sizeof(unsigned int));
		identity.resize(identity_size);
		file.read(&identity[0],identity_size);
		
		file.read(reinterpret_cast<char*>(&key_count),sizeof(unsigned int));

		for(unsigned int f=0;f<key_count;f++) {

			file.read(reinterpret_cast<char*>(&key_size),sizeof(unsigned int));
			key.resize(key_size);
			file.read(&key[0],key_size);
			file.read(reinterpret_cast<char*>(&value_size),sizeof(unsigned int));
			value.resize(value_size);
			file.read(&value[0],value_size);

			pass_data[key]=value;
		}

		try {
			entity_type_index.at(type);
		} catch(std::out_of_range) {
			GLPT_logger.Print("[GLPT_iterator] Map contained unknown entity type " + type);
			continue;
		}


		new_entity=entity_type_index[type]();

		this->Push(new_entity,identity,&pass_data);
	}

	GLPT_logger.Print("[GLPT_iterator] Finished loading!");

	file.close();
}