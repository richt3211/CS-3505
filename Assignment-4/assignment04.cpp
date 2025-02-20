/*
 * Assignment 04 -- An exploration of polymorphism in multiple
 * inheritance.  This code is instrumented to allow the user to see
 * function calls as they happen.  
 *
 * Students need to supply the missing class declarations, constructors,
 * and destructors, as well as any missing or otherwise required functions.
 * When you're done, the output should match the sample output --exactly--.
 * 
 * Find the TODO: comments, and replace them with the relevant code.  Do not
 * make changes to existing code (unless there is a TODO comment).
 *
 * Note that while this code is motivated by a particular problem, 
 * much of the functionality is missing.  Students should not try to 
 * implement observer functionality or remote connections.
 *
 * Additionally, while you can make multiple inheritance compile, its much
 * tougher to make it work properly.  Students should not fix the 
 * deficient design here.  Get the code to compile and match the given
 * design.  Later, you will describe possible fixes in essay questions.
 *
 * Richard Timpson 
 * April 1, 2019
 */

#include <iostream>
#include <vector>

using namespace std;

class observer;  // Never defined or used.

/******************
 * My value class *
 ******************/


class value
{
private:
  int v;
public :
  value(int v);
  virtual ~value();

  virtual int get();
  virtual void set(int v);
};

value::value(int v)
{
  cout << "      ==> value::value" << endl;
  cout << "      <-- value::value" << endl;
}

value::~value()
{
  cout << "      ==> value::~value" << endl;
  cout << "      <-- value::~value" << endl;
}


void value::set (int v)
{
  cout << "      ==> value::set" << endl;

  this->v = v;

  cout << "      <-- value::set" << endl;
}
 
int value::get ()
{
  cout << "      ==> value::get" << endl;
  cout << "      <-- value::get" << endl;
  return v;
}


/*************************
 * My remote value class *
 *************************/

class remote : public virtual value
{
private:
  bool remote_has_changed();
  void set_remote_value(int v);
  int get_remote_value();
public:
  remote(int v);
  virtual ~remote();

  void set(int v);
  int get();
};

remote::remote(int v)
  : value(v)
{
  cout << "    ==> remote::remote" << endl;
  cout << "    <-- remote::remote" << endl;
}

remote::~remote()
{
  cout << "    ==> remote::~remote" << endl;
  cout << "    <-- remote::~remote" << endl;
}


void remote::set (int v)
{
  cout << "    ==> remote::set" << endl;

  // Update the remote value and keep it as well.

  this->set_remote_value (v);
  value::set(v);              // Change superclass field

  cout << "    <-- remote::set" << endl;
}

int remote::get ()
{
  cout << "    ==> remote::get" << endl;

  // If the remote value has changed, we'll update this
  // object to the remote value before returning it.

  // To avoid setting the remote value, only set the value
  //   in the superclass.  (Don't use the 'set' function in
  //   this class, or it will set the remote value to what we
  //   just got remotely, which is a waste of time.)
  
  if (remote_has_changed())
    value::set(this->get_remote_value());

  // Just get the value (from the superclass) and return it.

  int result = value::get();

  cout << "    <-- remote::get" << endl;

  return result;
}

bool remote::remote_has_changed()
{
  cout << "    ==> remote::remote_has_changed" << endl;
  cout << "    <-- remote::remote_has_changed" << endl;

  return false;  // Just a stub -- assume that true might be returned.
}

int remote::get_remote_value()
{
  cout << "    ==> remote::get_remote_value" << endl;
  cout << "    <-- remote::get_remote_value" << endl;

  return 42;  // Just a stub -- an appropriate integer may be returned.
}

void remote::set_remote_value(int v)
{
  cout << "    ==> remote::set_remote_value" << endl;
  cout << "    <-- remote::set_remote_value" << endl;
}


/***********************
 * My observable class *
 ***********************/

class observable : public virtual value 
{
public:
  observable(int v);
  virtual ~observable();
  void register_observer(observer *);
  void set(int v);
private:
  void notify_observers();
};

observable::observable(int v)
  : value(v)
{
  cout << "    ==> observable::observable" << endl;
  cout << "    <-- observable::observable" << endl;
}

observable::~observable()
{
  cout << "    ==> observable::~observable" << endl;
  cout << "    <-- observable::~observable" << endl;
}




void observable::set (int new_value)
{
  cout << "    ==> observable::set" << endl;

  // Only change the value and send out notifications 
  //   if the new value is different than the current value in 'this'.

  if (new_value != this->get() ) 
    {
      value::set(new_value);
      notify_observers ();
    }

  cout << "    <-- observable::set" << endl;
}

void observable::notify_observers ()
{
  cout << "    ==> observable::notify_observers" << endl;
  cout << "    <-- observable::notify_observers" << endl;
}


void observable::register_observer (observer *)
{
  cout << "    ==> observable::register_observer" << endl;
  cout << "    <-- observable::register_observer" << endl;
}


/**************************
 * My task_priority class *
 **************************/

class task_priority : public remote, public observable
{
public:
  task_priority(int v);
  ~task_priority();
  void set(int v);

};

task_priority::task_priority(int v)
  : remote(v), observable(v), value(v)
{
  cout << "  ==> task_priority::task_priority" << endl;
  cout << "  <-- task_priority::task_priority" << endl;
}

task_priority::~task_priority()
{
  cout << "  ==> task_priority::~task_priority" << endl;
  cout << "  <-- task_priority::~task_priority" << endl;
}

void task_priority::set(int v)
{
  cout << "  ==> task_priority::set" << endl;
  observable::set(v);
  remote::set(v);
  cout << "  <-- task_priority::set" << endl;
}



/********************
 * My main (tester) *
 ********************/

int main ()
{
  // Make one of our objects.

  cout << "Creating a task_priority object:" << endl;
  cout << "--------------------------------" << endl;

  task_priority * task = new task_priority(50);

  cout << endl;
  
  // Add our task's pointer to a few vectors.  This
  // will demonstrate polymorphism.

  vector<task_priority*> vector_t;
  vector<remote*>        vector_r;
  vector<observable*>    vector_o;
  vector<value*>         vector_v;

  vector_t.push_back(task);
  vector_r.push_back(task);
  vector_o.push_back(task);
  vector_v.push_back(task);

  // Use the pointer from each vector.  Set a value,
  //   then get the value.  Because of polymorphism,
  //   this will result in the exact same sequence
  //   of function calls.

  int result;

  // First, use a task_priority pointer.

  cout << "Setting the priority to 60 (through a task_priority *):" << endl;
  cout << "-------------------------------------------------------" << endl;

  task_priority *t = vector_t[0];
  t->set(60);

  cout << endl;

  result = t->get();

  cout << endl;
  cout << "Priority is now " << result << "." << endl; 
  cout << endl;

  // Next, use a remote pointer

  cout << "Setting the priority to 30 (through a remote *):" << endl;
  cout << "------------------------------------------------" << endl;

  remote *r = vector_r[0];
  r->set(30);

  cout << endl;

  result = r->get();

  cout << endl;
  cout << "Priority is now " << result << "." << endl; 
  cout << endl;

  // Next, use an observable pointer

  cout << "Setting the priority to 75 (through an observable *):" << endl;
  cout << "-----------------------------------------------------" << endl;

  observable* o = vector_o[0];
  o->set(75);

  cout << endl;

  result = o->get();

  cout << endl;
  cout << "Priority is now " << result << "." << endl; 
  cout << endl;

  // Finally, use a value pointer

  cout << "Setting the priority to 22 (through a value *):" << endl;
  cout << "-----------------------------------------------" << endl;

  value* v = vector_v[0];
  v->set(22);

  cout << endl;

  result = v->get();

  cout << endl;
  cout << "Priority is now " << result << "." << endl; 
  cout << endl;

  // Done.  Delete our object (once only).  For fun, delete it
  //   using one of the base class pointers.  (We should still
  //   see all the destructors called.)
 
  cout << "Cleaning up:" << endl;
  cout << "------------" << endl;

  delete v;

  cout << endl;

  return 0;
}


