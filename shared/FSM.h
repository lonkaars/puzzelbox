#ifndef _FSM_H_
#define _FSM_H_

#include <map>
#include <memory>

#include "IBehaviouralState.hpp"

/// <summary>
///
/// </summary>
/// <typeparam name="TState">IBehaviouralState is the only accepted
/// class.</typeparam>
template <class TState> class FSM {
public:
	template <class... TPState> FSM(TPState &... args) {
		int i = 0;

		((void) _states.emplace(i++, args), ...);
	}

	/// <summary>
	/// Implement with FSM::act()
	/// </summary>
	void act();

	/// <summary>
	/// Used to check current state.
	/// </summary>
	/// <returns>Current state.</returns>
	std::shared_ptr<TState> & get_state() { return _currentState; }

	/// <summary>
	/// Used to get all states.
	/// </summary>
	/// <returns>Current states.</returns>
	std::map<int, std::shared_ptr<TState>> get_states();

	/// <summary>
	/// Sets current state, calls appropiate functions.
	/// </summary>
	/// <param name="">State to transition into.</param>
	void set_state(std::shared_ptr<TState>);

private:
	std::map<int, std::shared_ptr<TState>> _states;
	std::shared_ptr<TState> _currentState;
};

#endif // _FSM_H_