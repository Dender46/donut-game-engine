#pragma once
#include <stdint.h>
#include <vector>

namespace Donut {

	struct BaseECSComponent;

	typedef void* EntityHandle;
	typedef uint32_t(*ECSComponentCreateFunction)(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* component);
	typedef void (*ECSComponentFreeFunction)(BaseECSComponent* component);

	struct BaseECSComponent
	{
	public:
		static uint32_t RegisterComponent(ECSComponentCreateFunction createfn, ECSComponentFreeFunction freefn, size_t size);
		static inline ECSComponentCreateFunction GetCreateFunctionOfType(uint32_t componentType)
		{
			return std::get<0>((*s_ComponentTypes)[componentType]);
		}
		static inline ECSComponentFreeFunction GetFreeFunctionOfType(uint32_t componentType)
		{
			return std::get<1>((*s_ComponentTypes)[componentType]);
		}
		static inline size_t GetSizeOfType(uint32_t componentType)
		{
			return std::get<2>((*s_ComponentTypes)[componentType]);
		}

		EntityHandle Entity;
	private:
		// This vector is used for retrieveing information about specific component type
		static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* s_ComponentTypes;
	};

	// This type uses "Curiously recurring template pattern"
	template<typename T>
	struct ECSComponent : public BaseECSComponent
	{
		static const ECSComponentCreateFunction CREATE_FUNCTION;
		static const ECSComponentFreeFunction FREE_FUNCTION;
		static const uint32_t ID;
		static const size_t SIZE;
	};
	
	template<typename Component>
	uint32_t ECSComponentCreate(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* component)
	{
		size_t index = memory.size();
		memory.resize(index + Component::SIZE);
		//copy provided component to provided memory field
		Component* componentCopy = new(&memory[index]) Component(*(Component*)component);
		componentCopy->Entity = entity;
		return (uint32_t)index;
	}

	template<typename Component>
	void ECSComponentFree(BaseECSComponent* component)
	{
		Component* comp = (Component*)component;
		comp->~Component();
	}

	template<typename T>
	const uint32_t ECSComponent<T>::ID(BaseECSComponent::RegisterComponent
	(
		ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T))
	);

	template<typename T>
	const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

	template<typename T>
	const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

	template<typename T>
	const size_t ECSComponent<T>::SIZE(sizeof(T));

}