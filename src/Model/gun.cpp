#include "../../include/Common/common.h"
#include "../../include/Model/gun.h"
#include "gun.h"

Gun::Gun(EventBus &bus) : bus(bus){

}

bool Gun::getBulletType(int index){
  return bullets[index];
}

void Gun::invertCurrentBulletType(){
  if(!bullets.empty()) {
    bullets.back() = !bullets.back();
  }
}

void Gun::shoot(std::string username, std::string targetname){
  if(!bullets.empty()) {
    if((bullets.back() && targetname == username) || (!bullets.back() && targetname != username))
      bus.publish(std::make_shared<const OperatorChangeEvent>(username, targetname));
    if(bullets.back())
      bus.publish(std::make_shared<const HealthEvent>(cut?2:1, targetname)); 
    bullets.pop_back();
  }
}

void Gun::onFireEvent(std::shared_ptr<FireEvent> event){
  shoot(event->getUsername(), event->getTargetname());
}

void Gun::onItemUsedSawEvent() {
  //cut = true;
  throw std::runtime_error("Not implemented for the first demo");
}

void Gun::subscribe(EventBus &bus){
  std::weak_ptr<Gun> w_self = shared_from_this();
  bus.subscribe<FireEvent>([w_self](ConstEventPtr e) {
    if (auto self = w_self.lock()) { 
      auto fireEvent = std::static_pointer_cast<FireEvent>(e);
      self->onFireEvent(fireEvent);
    } 
  });
}
