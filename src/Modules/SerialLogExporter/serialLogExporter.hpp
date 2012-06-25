#ifndef SERIALLOGEXPORTER_HPP_
#define SERIALLOGEXPORTER_HPP_

#include "Framework/Module/vmodule.hpp"
#include "Framework/Notifications/notifier.hpp"
#include "Framework/Logging/logging.hpp"

namespace SerialLogExporter{

  /**
   * @brief Simple exporter for log notifications to the serial console
   *
   * @pre This module does not depend on any other module
   */
  class SerialLogExporter : public fwk::VModuleThread<512> {

  public:
    virtual msg_t init();

    virtual msg_t Main();

    virtual ~SerialLogExporter(){};

  private:
    fwk::StaticListener<fwk::LoggingMsg, 2> listener;

  };

}


#endif /* SERIALLOGEXPORTER_HPP_ */
