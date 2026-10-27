#pragma once

namespace Baba {

class IObserver {
  public:
    virtual ~IObserver() = default;
    virtual void OnBoardChanged() = 0;
};

}
