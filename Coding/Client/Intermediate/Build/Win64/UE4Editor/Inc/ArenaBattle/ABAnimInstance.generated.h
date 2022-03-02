// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef ARENABATTLE_ABAnimInstance_generated_h
#error "ABAnimInstance.generated.h already included, missing '#pragma once' in ABAnimInstance.h"
#endif
#define ARENABATTLE_ABAnimInstance_generated_h

#define Client_Source_ArenaBattle_ABAnimInstance_h_15_SPARSE_DATA
#define Client_Source_ArenaBattle_ABAnimInstance_h_15_RPC_WRAPPERS
#define Client_Source_ArenaBattle_ABAnimInstance_h_15_RPC_WRAPPERS_NO_PURE_DECLS
#define Client_Source_ArenaBattle_ABAnimInstance_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUABAnimInstance(); \
	friend struct Z_Construct_UClass_UABAnimInstance_Statics; \
public: \
	DECLARE_CLASS(UABAnimInstance, UAnimInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/ArenaBattle"), NO_API) \
	DECLARE_SERIALIZER(UABAnimInstance)


#define Client_Source_ArenaBattle_ABAnimInstance_h_15_INCLASS \
private: \
	static void StaticRegisterNativesUABAnimInstance(); \
	friend struct Z_Construct_UClass_UABAnimInstance_Statics; \
public: \
	DECLARE_CLASS(UABAnimInstance, UAnimInstance, COMPILED_IN_FLAGS(0 | CLASS_Transient), CASTCLASS_None, TEXT("/Script/ArenaBattle"), NO_API) \
	DECLARE_SERIALIZER(UABAnimInstance)


#define Client_Source_ArenaBattle_ABAnimInstance_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UABAnimInstance(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UABAnimInstance) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UABAnimInstance); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UABAnimInstance); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UABAnimInstance(UABAnimInstance&&); \
	NO_API UABAnimInstance(const UABAnimInstance&); \
public:


#define Client_Source_ArenaBattle_ABAnimInstance_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UABAnimInstance(UABAnimInstance&&); \
	NO_API UABAnimInstance(const UABAnimInstance&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UABAnimInstance); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UABAnimInstance); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(UABAnimInstance)


#define Client_Source_ArenaBattle_ABAnimInstance_h_15_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CurrentPawnSpeed() { return STRUCT_OFFSET(UABAnimInstance, CurrentPawnSpeed); } \
	FORCEINLINE static uint32 __PPO__IsInAir() { return STRUCT_OFFSET(UABAnimInstance, IsInAir); } \
	FORCEINLINE static uint32 __PPO__AttackMontage() { return STRUCT_OFFSET(UABAnimInstance, AttackMontage); }


#define Client_Source_ArenaBattle_ABAnimInstance_h_12_PROLOG
#define Client_Source_ArenaBattle_ABAnimInstance_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_PRIVATE_PROPERTY_OFFSET \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_SPARSE_DATA \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_RPC_WRAPPERS \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_INCLASS \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define Client_Source_ArenaBattle_ABAnimInstance_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_PRIVATE_PROPERTY_OFFSET \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_SPARSE_DATA \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_INCLASS_NO_PURE_DECLS \
	Client_Source_ArenaBattle_ABAnimInstance_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ARENABATTLE_API UClass* StaticClass<class UABAnimInstance>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID Client_Source_ArenaBattle_ABAnimInstance_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
