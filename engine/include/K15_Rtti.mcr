#define DECLARE_RTTI \
public:	\
			static const K15_Engine::System::Rtti TYPE;	\
			virtual const K15_Engine::System::Rtti &getType() const{ return TYPE; }	

#define DECLARE_RTTI_TEMPLATE \
public: \
			static const K15_Engine::System::Rtti TYPE; \
			virtual const K15_Engine::System::Rtti &getType() const{ return TYPE; }	

#define IMPLEMENT_RTTI_BASE(namespace,classtype,basetype)	\
		const K15_Engine::System::Rtti classtype::TYPE(#namespace"."#classtype,&basetype::TYPE);

#define IMPLEMENT_RTTI(namespace,classtype)	\
		const K15_Engine::System::Rtti classtype::TYPE(#namespace"."#classtype,0);

#define IMPLEMENT_RTTI_TEMPLATE_BASE(namespace,classtype,basetype) \
		template<> \
			const K15_Engine::System::Rtti classtype::TYPE(#namespace"."#classtype,&basetype::TYPE);

#define IMPLEMENT_RTTI_TEMPLATE(namespace,classtype) \
		templace<> \
			const K15_Engine::System::Rtti classtype::TYPE(#namespace"."#classtype,0);