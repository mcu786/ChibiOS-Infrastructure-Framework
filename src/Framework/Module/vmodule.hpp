/*
 * vmodule.hpp
 *
 *  Created on: Apr 12, 2012
 *      Author: mabl
 */

#ifndef VMODULE_HPP_
#define VMODULE_HPP_

#include "ch.h"

#include "Utilities/singleton.hpp"
#include "Framework/ChibiOS/ch_wrapper.hpp"

/**
 * @brief Get a reference to the module of name @p module.
 *
 * @details This method is implemented via a Meyers singleton.
 */
#define VMODULE_GET_MODULE_REF(module)                                       \
                             ( utl::Singleton<module::module>::GetInstance() )


namespace fwk {

  /**
   * @brief VModule provides the abstract module interface.
   */
  class VModule {
  public:

    /**
     * @brief Initialize module.
     *
     * @details Since the modules are typically static global objects it is
     *          best to move all ChibiOS and Framework dependent
     *          initialization out of the constructor.
     *
     * @return  0   If no error.
     * @return !=0  For any error. Error code is module dependent.
     */
    virtual msg_t init() = 0;

    /**
     * @brief Run method of the module.
     *
     * @note This method is not supposed to stall the execution.
     *
     * @return  0   If no error.
     * @return !=0  For any error. Error code is module dependent.
     */
    virtual msg_t start() = 0;


  protected:
    /**
     * @brief Constructor and destructor are protected to prevent unwanted
     *        creation.
     */
    VModule(){};
    virtual ~VModule(){};


    friend class utl::Singleton<VModule>;

  };

  /**
   * @brief Threaded Module which runs its own thread
   *
   * @details Modules must implement the @p Main() method of @p EnhancedThread.
   */
  template<int WASize>
  class VModuleThread : public VModule ,
                         protected chibios_rt::EnhancedThread<WASize>{

  public:
    VModuleThread() : prio(NORMALPRIO) {}
    VModuleThread(tprio_t prio) : prio(prio) {}

    virtual msg_t start();

  private:
    tprio_t prio;
  };

}


template<int WASize>
msg_t fwk::VModuleThread<WASize>::start() {
  this->Start(prio);

  return RDY_OK;
}


#endif /* VMODULE_HPP_ */
