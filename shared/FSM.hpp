#ifndef _FSM_HPP_
#define _FSM_HPP_

#include "FSM.h"

#include <iostream>

/// <summary>
/// This is a generic update method, unknown classes are not supported.
/// </summary>
/// <typeparam name="TState">IBehaviouralState is the only accepted
/// class.</typeparam>
template <class TState> void FSM<TState>::act() {
  // No implementation for unknown class.
  std::cout << "No implementation for unknown class" << std::endl;
}
//
///// <summary>
///// Calls the IBehaviouralState act method.
///// </summary>
///// <typeparam name="TState">IBehaviouralState is the only accepted
/// class.</typeparam>
template <> inline void FSM<IBehaviouralState>::act() { _currentState->act(); }

/// <summary>
/// This is a generic return of the type.
/// </summary>
/// <typeparam name="TState">Any class type works with this return.</typeparam>
/// <returns>The current IBehaviouralState map.</returns>
template <class TState>
std::map<int, std::shared_ptr<TState>> FSM<TState>::get_states() {
  return _states;
}

///// <summary>
///// State transitioning from current state to newState.
///// Calls Exit on the current state and Enter on the new state.
///// </summary>
///// <typeparam name="TState">IBehaviouralState is the only accepted
/// class.</typeparam>
///// <param name="newState">New state to transition into.</param>
template <>
inline void
FSM<IBehaviouralState>::set_state(std::shared_ptr<IBehaviouralState> newState) {
  // We can guarantee all statemachines are based on IBehaviouralState
  if (_currentState != nullptr)
    _currentState->exit();

  _currentState = nullptr;
  _currentState = newState;
  _currentState->enter();
}

/// <summary>
/// State transitioning from current state to newState.
/// Calls Exit on the current state and Enter on the new state.
/// </summary>
/// <typeparam name="TState">IBehaviouralState is the only accepted
/// class.</typeparam> <param name="newState">New state to transition
/// into.</param>
template <class TState>
void FSM<TState>::set_state(std::shared_ptr<TState> newState) {
  // We can guarantee all statemachines are based on IBehaviouralState
  // No implementation for an unknown state change class.
  std::cout << "No implementation for unknown state change class" << std::endl;
}

#endif // _FSM_HPP_