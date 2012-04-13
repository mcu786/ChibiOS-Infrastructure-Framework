#ifndef _utl_Singleton_h_
#define _utl_Singleton_h_


namespace utl {

  /**
   * \class Singleton Singleton.h utl/Singleton.h
   *
   * \brief Curiously Recurring Template Pattern (CRTP) for Meyers singleton
   *
   * The singleton class is implemented as follows
   * \code
   * #include <utl/Singleton.h>
   *
   * class SomeClass : public utl::Singleton<SomeClass> {
   *   ...
   * private:
   *   // prevent creation, destruction
   *   SomeClass() { }
   *   ~SomeClass() { }
   *
   *   friend class utl::Singleton<SomeClass>;
   * };
   * \endcode
   * Singleton automatically prevents copying of the derived class.
   */

  template<typename T>
  class Singleton {
  public:
    static T& GetInstance(){
      static T instance;
      return instance;
    }

  protected:
    // derived class can call ctor and dtor
    Singleton() { }
    ~Singleton() { }

  private:
    // no one should do copies
    Singleton(const Singleton&);
    Singleton& operator=(const Singleton&);

  };

}


#endif
