#include "ElevatorState.h"
#include "PreconditionException.h"

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

typedef ElevatorState State;

struct ProgramState;

class Program
{
public:
    typedef shared_ptr<Program> Ptr;

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
            for(auto& x : ps.trans())
            {
                s.push(x);
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

class Empty : public Program
{
public:
    Empty() = default;

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

Program::Ptr empty()
{
    return make_shared<Empty>();
}

class Exec : public Program
{
public:
    Exec(function<State::Ptr(State::Ptr)> action_)
        : action(action_)
    {
    }

    virtual vector<ProgramState> trans(State::Ptr state)
    {
        vector<ProgramState> ret;
        try
        {
            ret.push_back(ProgramState(empty(), action(state)));
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
    function<State::Ptr(State::Ptr)> action;
};

class Test : public Program
{
public:
    Test(function<bool(State::Ptr)> predicate_)
        : predicate(predicate_)
    {
    }

    virtual vector<ProgramState> trans(State::Ptr state)
    {
        vector<ProgramState> ret;
        if(predicate(state))
        {
            ret.push_back(ProgramState(empty(), state));
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
    function<bool(State::Ptr)> predicate;
};

class Sequence : public Program
{
public:
    Sequence(Program::Ptr p1_, Program::Ptr p2_)
        : p1(p1_), p2(p2_)
    {
    }

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
                ret.push_back(ProgramState(make_shared<Sequence>(ps.program, p2), ps.state));
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

class Choose : public Program
{
public:
    Choose(Program::Ptr p1_, Program::Ptr p2_)
        : p1(p1_), p2(p2_)
    {
    }

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

class Star : public Program
{
public:
    Star(Program::Ptr p1_)
        : p1(p1_)
    {
    }

    virtual vector<ProgramState> trans(State::Ptr state)
    {
        vector<ProgramState> ret;
        ret.push_back(ProgramState(make_shared<Choose>(empty(), make_shared<Sequence>(p1, make_shared<Star>(p1))), state));
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

int main()
{
    typedef ElevatorState State;

    int at = 0;
    set<int> lights = {3, 5};
    State::Ptr s0 = make_shared<State>(at, lights);
    cout << "s0 = " << *s0 << endl;

    State::Ptr s1 = s0->up()->up()->up()->turnOff();
    cout << "s1 = s0->up()->up()->up()->turnOff() = " << *s1 << endl;

    State::Ptr s2 = s1->down()->down();
    cout << "s2 = s1->down()->down() = " << *s2 << endl;

    typedef Program P;
    P::Ptr at1 = make_shared<Test>([](State::Ptr s) -> bool {return s->at == 1;});
    P::Ptr down = make_shared<Exec>([](State::Ptr s) -> State::Ptr {return s->down();});
    P::Ptr at0 = make_shared<Test>([](State::Ptr s) -> bool {return s->at == 0;});
    P::Ptr up = make_shared<Exec>([](State::Ptr s) -> State::Ptr {return s->up();});
    P::Ptr up2 = make_shared<Sequence>(up, up);
    P::Ptr up3 = make_shared<Sequence>(up, up2);
    P::Ptr up4 = make_shared<Sequence>(up, up3);
    P::Ptr up5 = make_shared<Sequence>(up, up4);
    P::Ptr seq1 = make_shared<Sequence>(at1, down);
    P::Ptr seq2 = make_shared<Sequence>(at0, up);
    P::Ptr p1 = make_shared<Choose>(seq1, seq2);

    P::Ptr p = make_shared<Sequence>(
            //make_shared<Star>(up),
            make_shared<Choose>(
                up,
                make_shared<Choose>(
                    up2,
                    make_shared<Choose>(
                        up3,
                        make_shared<Choose>(
                            up4,
                            up5
                        )
                    )
                )
            ),
            make_shared<Test>([](State::Ptr s) -> bool {
                return s->at >= 3 && s->at <= 5;
            }));
    ProgramState ps0(p, s0);
    cout << "ps0 = " << ps0 << endl;
    ProgramStateIterator it(ps0);
    while(it.hasNext())
    {
        ProgramState ps = it.next();
        cout << "transStar: s = " << *ps.state << endl;
    }
}

