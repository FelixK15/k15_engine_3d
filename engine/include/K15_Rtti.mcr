#define DECLARE_RTTI \
public:	\
			static const K15_EngineV2::Rtti TYPE;	\
			virtual const K15_EngineV2::Rtti &GetType() const{ return TYPE; }	

#define DECLARE_RTTI_TEMPLATE \
public: \
			K15ENGINE2_API static const K15_EngineV2::Rtti TYPE; \
			virtual const K15_EngineV2::Rtti &GetType() const{ return TYPE; }	

#define IMPLEMENT_RTTI(namespace,classtype,basetype)	\
		const K15_EngineV2::Rtti classtype::TYPE(#namespace"."#classtype,&basetype::TYPE);

#define IMPLEMENT_RTTI_TEMPLATE(namespace,classtype,basetype) \
		template<> \
			const K15_EngineV2::Rtti classtype::TYPE(#namespace"."#classtype,&basetype::TYPE);