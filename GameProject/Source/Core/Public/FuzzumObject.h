// FuzzumObject.h
#pragma once
#include <vector>
#include <functional>
#include <string>
#include <unordered_map>

// 🔑 Forward-декларации
namespace fuzzum {
    class FuzzumObject; // ← объявляем ДО использования

    struct FuzzumProp {
        const char* Name;
        size_t Offset;
        const char* TypeName;
        const char* DisplayName;
        bool bReplicated;
        bool bSaveGame;
        const char* Category;
    };

    // Теперь FuzzumObject известен
    using FuzzumClassFactory = FuzzumObject*(*)();

    struct FuzzumClassInfo {
        const char* ClassName;
        const char* SuperClassName;
        FuzzumClassFactory Factory;
        std::function<void(FuzzumObject*, std::function<void(const FuzzumProp&, void*)>)> VisitPropsFunc;
    };

    extern std::unordered_map<std::string, FuzzumClassInfo> GClassRegistry;

    template<typename T>
    struct FuzzumMeta {
        static std::vector<FuzzumProp>& Get();
    };

    class FuzzumObject {
    public:
        virtual ~FuzzumObject() = default;
        virtual const char* StaticTypeId() const = 0;
        virtual void VisitProps(std::function<void(const FuzzumProp&, void*)> fn) = 0;
    };

    // Макросы (без исключений!)
    #define FUZZUM_BODY() \
        friend struct fuzzum::FuzzumMeta<std::decay_t<decltype(*this)>>; \
        static const char* StaticTypeId();

    #define FUZZUM_BODY_WITH_BASE(BaseClass) \
        using Super = BaseClass; \
        friend struct fuzzum::FuzzumMeta<std::decay_t<decltype(*this)>>; \
        static const char* StaticTypeId();

    #define FUZZUM_REGISTER_CLASS_WITH_SUPER(ClassName, SuperClassName) \
    namespace { \
        fuzzum::FuzzumObject* Create##ClassName() { \
            return new ClassName(); \
        } \
        void VisitPropsFunc_##ClassName(fuzzum::FuzzumObject* obj, std::function<void(const fuzzum::FuzzumProp&, void*)> fn) { \
            static_cast<ClassName*>(obj)->VisitProps(fn); \
        } \
        struct AutoRegister_##ClassName { \
            AutoRegister_##ClassName() { \
                const char* superName = (std::is_same_v<Super, fuzzum::FuzzumObject> || \
                                        std::string(#SuperClassName) == "fuzzum::FuzzumObject") \
                                        ? nullptr : #SuperClassName; \
                fuzzum::GClassRegistry[#ClassName] = fuzzum::FuzzumClassInfo{ \
                    #ClassName, superName, &Create##ClassName, &VisitPropsFunc_##ClassName \
                }; \
            } \
        }; \
        static AutoRegister_##ClassName _auto_reg_##ClassName; \
    }

// Для корневых классов (наследующих напрямую от FuzzumObject)
#define FUZZUM_REGISTER_CLASS(ClassName) \
    FUZZUM_REGISTER_CLASS_WITH_SUPER(ClassName, fuzzum::FuzzumObject)

    // Совместимость с C++17 (без requires)
#define FUZZUM_IMPLEMENT_VISIT_PROPS(ClassName) \
        void VisitProps(std::function<void(const fuzzum::FuzzumProp&, void*)> fn) override { \
            if constexpr (!std::is_same_v<Super, fuzzum::FuzzumObject>) { \
                Super::VisitProps(fn); \
            } \
            for (auto& prop : fuzzum::FuzzumMeta<ClassName>::Get()) { \
                void* addr = reinterpret_cast<char*>(this) + prop.Offset; \
                fn(prop, addr); \
            } \
        }

    // Функции без исключений
    FuzzumObject* NewObject(const char* className);
    FuzzumObject* NewObject(const std::string& className);
    bool ClassExists(const char* className);
    bool ClassExists(const std::string& className);
} // namespace fuzzum