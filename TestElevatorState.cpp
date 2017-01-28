#include "ElevatorState.h"
#include "PreconditionException.h"
#include "Util.h"

#include <functional>
#include <utility>
#include <vector>
#include <stack>
#include <iostream>

using std::cout;
using std::endl;
using std::function;
using std::bind;
using std::pair;
using std::make_pair;
using std::placeholders::_1;
using std::vector;
using std::stack;
using std::cout;
using std::endl;

using State = ElevatorState;
using Action = function<State::Ptr(State::Ptr)>;
using Predicate = function<bool(State::Ptr)>;

struct ProgramState;

class Program
{
public:
    typedef std::shared_ptr<Program> Ptr;
    
    virtual vector<ProgramState> trans(State::Ptr state) = 0;

    virtual bool isFinal(State::Ptr state) = 0;

    virtual string str() const = 0;
};

ostream& operator<<(ostream& os, const ProgramState& ps);
ostream& operator<<(ostream& os, const Program& p);

struct ProgramState
{
public:
    ProgramState(Program::Ptr program_, State::Ptr state_) : program(program_), state(state_) {}
    Program::Ptr program;
    State::Ptr state;
    vector<string> actions;

    vector<ProgramState> trans()
    {
        return program->trans(state);
    }

    bool isFinal()
    {
        return program->isFinal(state);
    }

    string str() const
    {
        string s = "ProgramState(";
        s += program->str();
        s += ", ";
        s += state->str();
        s += ")";
        return s;
    }
};

class ProgramStateIterator
{
private:
    stack<ProgramState> s;

    void expandTop()
    {
        while(!s.empty() && !s.top().isFinal())
        {
            ProgramState ps = s.top();
            s.pop();
            //cout << "ProgramStateIterator:  pop " << *ps.program << "  " << *ps.state << endl;
            for(auto& x : ps.trans())
            {
                s.push(x);
                //cout <<"ProgramStateIterator: push " << *x.program << "  " << *x.state << endl;
            }
        }
    }

public:
    ProgramStateIterator(ProgramState ps0)
    {
        s.push(ps0);
    }

    bool hasNext()
    {
        expandTop();
        return !s.empty();
    }

    ProgramState next()
    {
        expandTop();
        ProgramState ps = s.top();
        s.pop();
        return ps;
    }
};

class Empty : public Program, public add_make_shared<Empty>
{
protected:
    friend class add_make_shared<Empty>;
    
    Empty() = default;

public:
    virtual vector<ProgramState> trans(State::Ptr state)
    {
        return vector<ProgramState>();
    }

    virtual bool isFinal(State::Ptr state)
    {
        return true;
    }

    virtual string str() const
    {
        return "Empty()";
    }
};

class Exec : public Program, public add_make_shared<Exec>
{
private:
    friend class add_make_shared<Exec>;

    Exec(Action action_)
        : action(action_)
    {
    }

public:
    virtual vector<ProgramState> trans(State::Ptr state)
    {
        vector<ProgramState> ret;
        try
        {
            ret.push_back(ProgramState(Empty::make_shared(), action(state)));
        }
        catch(PreconditionException& ex)
        {
        }
        return ret;
    }

    virtual bool isFinal(State::Ptr state)
    {
        return false;
    }

    virtual string str() const
    {
        return "Exec(...)";
    }

private:
    Action action;
};

class Test : public Program, public add_make_shared<Test>
{
private:
    friend class add_make_shared<Test>;

    Test(Predicate predicate_)
        : predicate(predicate_)
    {
    }

public:
    virtual vector<ProgramState> trans(State::Ptr state)
    {
        vector<ProgramState> ret;
        if(predicate(state))
        {
            ret.push_back(ProgramState(Empty::make_shared(), state));
        }
        return ret;
    }

    virtual bool isFinal(State::Ptr state)
    {
        return false;
    }

    virtual string str() const
    {
        return "Test(...)";
    }

private:
    Predicate predicate;
};

class Sequence : public Program, public add_make_shared<Sequence>
{
private:
    friend class add_make_shared<Sequence>;

    Sequence(Program::Ptr p1_, Program::Ptr p2_)
        : p1(p1_), p2(p2_)
    {
    }

public:
    virtual vector<ProgramState> trans(State::Ptr state)
    {
        vector<ProgramState> ret;
        if(p1->isFinal(state))
        {
            for(auto& ps : p2->trans(state))
                ret.push_back(ps);
        }
        else
        {
            for(auto& ps : p1->trans(state))
                ret.push_back(ProgramState(Sequence::make_shared(ps.program, p2), ps.state));
        }
        return ret;
    }

    virtual bool isFinal(State::Ptr state)
    {
        return p1->isFinal(state) && p2->isFinal(state);
    }

    virtual string str() const
    {
        string s = "Sequence(";
        s += p1->str();
        s += ", ";
        s += p2->str();
        s += ")";
        return s;
    }

private:
    Program::Ptr p1;
    Program::Ptr p2;
};

class Choose : public Program, public add_make_shared<Choose>
{
private:
    friend class add_make_shared<Choose>;

    Choose(Program::Ptr p1_, Program::Ptr p2_)
        : p1(p1_), p2(p2_)
    {
    }

public:
    virtual vector<ProgramState> trans(State::Ptr state)
    {
        vector<ProgramState> ret;
        for(auto& ps : p1->trans(state))
            ret.push_back(ps);
        for(auto& ps : p2->trans(state))
            ret.push_back(ps);
        return ret;
    }

    virtual bool isFinal(State::Ptr state)
    {
        return p1->isFinal(state) || p2->isFinal(state);
    }

    virtual string str() const
    {
        string s = "Choose(";
        s += p1->str();
        s += ", ";
        s += p2->str();
        s += ")";
        return s;
    }

private:
    Program::Ptr p1;
    Program::Ptr p2;
};

class Star : public Program, public add_make_shared<Star>
{
private:
    friend class add_make_shared<Star>;

    Star(Program::Ptr p_)
        : p(p_)
    {
    }

public:
    virtual vector<ProgramState> trans(State::Ptr state)
    {
        vector<ProgramState> ret;
        for(auto& ps : p->trans(state))
            ret.push_back(ProgramState(Sequence::make_shared(ps.program, shared_from_this()), ps.state));
        ret.push_back(ProgramState(Empty::make_shared(), state));
        return ret;
    }

    virtual bool isFinal(State::Ptr state)
    {
        return false;
    }

    virtual string str() const
    {
        string s = "Star(";
        s += p->str();
        s += ")";
        return s;
    }

private:
    Program::Ptr p;
};

ostream& operator<<(ostream& os, const Program& p)
{
    os << p.str();
    return os;
}

ostream& operator<<(ostream& os, const ProgramState& ps)
{
    os << ps.str();
    return os;
}

// convenience shared_ptr constructors:

Program::Ptr empty() {
    return Empty::make_shared();
}

Program::Ptr exec(Action a) {
    return Exec::make_shared(a);
}

Program::Ptr test(Predicate p) {
    return Test::make_shared(p);
}

Program::Ptr seq(Program::Ptr p1, Program::Ptr p2) {
    return Sequence::make_shared(p1, p2);
}

Program::Ptr choose(Program::Ptr p1, Program::Ptr p2) {
    return Choose::make_shared(p1, p2);
}

Program::Ptr star(Program::Ptr p) {
    return Star::make_shared(p);
}

int main()
{
    typedef ElevatorState State;

    int at = 0;
    set<int> lights = {3, 5};
    State::Ptr s0 = State::make_shared(at, lights);

    Program::Ptr down = Exec::make_shared([](State::Ptr s) -> State::Ptr {return s->down();});
    Program::Ptr up = Exec::make_shared([](State::Ptr s) -> State::Ptr {return s->up();});
    Program::Ptr turnOff = Exec::make_shared([](State::Ptr s) -> State::Ptr {return s->turnOff();});
    Predicate allOff = [](State::Ptr s) -> bool {return s->lights.empty();};
    
    Program::Ptr serve_a_floor = choose(
        seq(star(up), turnOff),
        seq(star(down), turnOff)
    );
    
    Program::Ptr serve_all_floors = seq(
        star(serve_a_floor),
        test(allOff)
    );
    
    Program::Ptr p = serve_all_floors;

    const int maxSolutions = 10;
    int numSolutions = 0;
    ProgramStateIterator it(ProgramState(p, s0));
    while(it.hasNext())
    {
        ProgramState ps = it.next();
        cout << "transStar: s = " << *ps.state << endl;
        if(++numSolutions >= maxSolutions)
        {
            cout << "Showing only first " << maxSolutions << " solutions" << endl;
            break;
        }
    }
}

