#include "ElevatorState.h"
#include "ProgramStateIterator.h"
#include "Program.h"
#include "Empty.h"
#include "Exec.h"
#include "Test.h"
#include "Sequence.h"
#include "Star.h"
#include "Choose.h"

#include <iostream>
#include <string>
#include <map>
#include <stack>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::stack;

int main()
{
    using State = ElevatorState;
    using Program = Program<State>;

    int at = 0;
    set<int> lights = {3, 5};
    State::Ptr s0 = State::make_shared(at, lights);

    Program::Ptr down = exec<State>([](State::Ptr s) -> State::Ptr {return s->down();});
    Program::Ptr up = exec<State>([](State::Ptr s) -> State::Ptr {return s->up();});
    Program::Ptr turnOff = exec<State>([](State::Ptr s) -> State::Ptr {return s->turnOff();});
    Program::Ptr allOff = test<State>([](State::Ptr s) -> bool {return s->lights.empty();});
    
    Program::Ptr serve_a_floor = choose<State>(
        seq<State>(star<State>(up), turnOff),
        seq<State>(star<State>(down), turnOff)
    );
    
    Program::Ptr serve_all_floors = seq<State>(
        star<State>(serve_a_floor),
        allOff
    );
    
    map<Program::Ptr, string> actionName;
    actionName[up] = "up";
    actionName[down] = "down";
    actionName[turnOff] = "turnOff";
    
    Program::Ptr p = serve_all_floors;

    const int maxSolutions = 10;
    int numSolutions = 0;
    ProgramStateIterator<State> it(make_shared<ProgramState<State>>(p, s0));
    while(it.hasNext())
    {
        ProgramState<State>::Ptr ps = it.next();
        
        stack<string> plan;
        ProgramState<State>::Ptr tmp = ps;
        while(tmp)
        {
            if(tmp->action) plan.push(actionName[tmp->action]);
            tmp = tmp->parent;
        }
        
        cout << "transStar: s = " << *ps->state << ", plan = ";
        string sep = "";
        while(!plan.empty())
        {
            cout << sep << plan.top();
            plan.pop();
            sep = ", ";
        }
        cout << endl;

        if(++numSolutions >= maxSolutions)
        {
            cout << "Showing only first " << maxSolutions << " solutions" << endl;
            break;
        }
    }
}

