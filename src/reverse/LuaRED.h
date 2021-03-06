#pragma once

#include "common/Meta.h"

template<class T, FixedString REDName>
struct LuaRED
{
	static constexpr char const* Name = REDName;
	
	sol::object ToLua(RED4ext::REDreverse::CScriptableStackFrame::CStackType& aResult, sol::state_view aLua)
	{
		return make_object(aLua, *static_cast<T*>(aResult.value));
	}

	RED4ext::REDreverse::CScriptableStackFrame::CStackType ToRED(sol::object aObject, RED4ext::REDreverse::CRTTIBaseType* apRtti, TiltedPhoques::Allocator* apAllocator) 
	{
		RED4ext::REDreverse::CScriptableStackFrame::CStackType result;
		result.type = m_pRtti;
		if(aObject != sol::nil)
			result.value = apAllocator->New<T>(aObject.as<T>());
		else
			result.value = apAllocator->New<T>();

		return result;
	}

	size_t Size() const noexcept
	{
		return sizeof(T);
	}

	bool Is(RED4ext::REDreverse::CRTTIBaseType* apRtti) const
	{
		if (!Resolve())
			return false;
		
		return apRtti == m_pRtti;
	}

private:

	bool Resolve() const
	{
		if (m_pRtti)
			return true;

		auto* pRtti = RED4ext::REDreverse::CRTTISystem::Get();
		m_pRtti = pRtti->GetType(RED4ext::FNV1a(Name));

		return m_pRtti != nullptr;
	}

	mutable RED4ext::REDreverse::CRTTIBaseType* m_pRtti{nullptr};
};
