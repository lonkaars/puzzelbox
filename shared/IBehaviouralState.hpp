#ifndef _FSM_IBEHAVIOURALSTATE_HPP_
#define _FSM_IBEHAVIOURALSTATE_HPP_

/// <summary>
/// Sub class used to define methods implemented by behavioural specific states.
/// </summary>
class IBehaviouralState {
public:
  /// <summary>
  /// Enters the current state. Used for setup.
  /// </summary>
  virtual void enter() = 0;

  /// <summary>
  /// Updates the current state, used for physics, etc.
  /// </summary>
  virtual void act() = 0;

  /// <summary>
  /// Exits the state, used for cleanup.
  /// </summary>
  virtual void exit() = 0;
};

#endif // _FSM_IBEHAVIOURALSTATE_HPP_