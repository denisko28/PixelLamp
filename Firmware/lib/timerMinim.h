#include <Arduino.h>

class timerMinim
{
  public:
    timerMinim(int);				// объявление таймера с указанием интервала
    void setInterval(int);	// установка интервала работы таймера
    bool isReady();					        	// возвращает true, когда пришло время. Сбрасывается в false сам (AUTO) или вручную (MANUAL)
    void reset();						            	// ручной сброс таймера на установленный интервал

  private:
    int _timer = 0;
    int _interval = 0;
};

timerMinim::timerMinim(int interval) {
  _interval = interval;
  _timer = millis();
}

void timerMinim::setInterval(int interval) {
  _interval = interval;
}

bool timerMinim::isReady() {
  if ((long)millis() - _timer >= _interval) {
    _timer = millis();
    return true;
  } else {
    return false;
  }
}

void timerMinim::reset() {
  _timer = millis();
}