//////////////////// macros.h ///////////////// rado ////
//                                                            //
//  Copyright (c) 1999 Radoslav Getov                         //
//                                                            //
//  Permission to use, copy, modify and distribute this       //
//  software is granted without fee, provided that the above  //
//  copyright notice and this permission notice appear in the //
//  modified source and the source files that #include it.    //
//                                                            //
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//                                                            //
//  Collection of C and C++ preprocessor utility macros:      //
//                                                            //
//  ------ C and C++ -----                                    //
//  REPEAT_xxx    (text)                                      //
//  REPEAT_WC_xxx (text)                                      //
//  REPEAT        (count, text)                               //
//  REPEAT_WC     (count, text)                               //
//                                                            //
//  UNIQUE_NAME_WP (prefix)                                   //
//  UNIQUE_NAME                                               //
//                                                            //
//  LINE_STRING                                               //
//  HERE                                                      //
//                                                            //
//  ONCE     (execution_code)                                 //
//  SKIP     (skip_count, execution_code)                     //
//  REV_SKIP (skip_count, execution_code)                     //
//  LOOP_C   (count, execution_code)                          //
//                                                            //
//  ------ C++ only -----                                     //
//  LOOP (count)                                              //
//                                                            //
//  AT_START (execution_code)                                 //
//  AT_END   (execution_code)                                 //
//                                                            //
//  DELAYED_ASSIGN_T (Type, var_reference, value)             //
//  SAVE_T           (Type, var_reference)                    //
//  DELAYED_ASSIGN   (var_reference, value)                   //
//  SAVE             (var_reference)                          //
//                                                            //
////////////////////////////////////////////////////////////////
                                                              

// ------------- include guards -----------------             
                          
#ifndef ID3LIB_MACROS_H                                        
#define ID3LIB_MACROS_H

// ------------- portability --------------------

#ifdef __cplusplus  // defined for C++ mode compilation
#  define CPP_MODE_COMPILATION 1
#else
#  define CPP_MODE_COMPILATION 0
#endif

// --------------- includes ---------------------

#if CPP_MODE_COMPILATION
#  include <new>
#endif

// --------------- definitions ------------------


//===================| #REPEAT_xxx (text) |=====================
// Textually repeat argument xxx times.
//
// Example: char* spaces = REPEAT_32 (" ");
//________________________________________________________rado__
#define REPEAT_2(x)       x x
#define REPEAT_4(x)       REPEAT_2   (x)  REPEAT_2   (x)
#define REPEAT_8(x)       REPEAT_4   (x)  REPEAT_4   (x)
#define REPEAT_16(x)      REPEAT_8   (x)  REPEAT_8   (x)
#define REPEAT_32(x)      REPEAT_16  (x)  REPEAT_16  (x)
#define REPEAT_64(x)      REPEAT_32  (x)  REPEAT_32  (x)
#define REPEAT_128(x)     REPEAT_64  (x)  REPEAT_64  (x)
#define REPEAT_256(x)     REPEAT_128 (x)  REPEAT_128 (x)
#define REPEAT_512(x)     REPEAT_256 (x)  REPEAT_256 (x)
#define REPEAT_1024(x)    REPEAT_512 (x)  REPEAT_512 (x)


//===============| #REPEAT (count, what) |======================
// Textually repeat 'what' 'cnt' times.
//
// Example: char* spaces = REPEAT (32, " ");
//________________________________________________________rado__
#define REPEAT(count, what)   REPEAT_##count (what)


//====================| #REPEAT_WC_xxx (text) |=================
// Textually repeat argument xxx times, delimited by comas.
//
// Example: int ones[] = { REPEAT_WC_16 (1) };
//________________________________________________________rado__
#define REPEAT_WC_2(x)    x, x
#define REPEAT_WC_4(x)    REPEAT_WC_2   (x), REPEAT_WC_2   (x)  
#define REPEAT_WC_8(x)    REPEAT_WC_4   (x), REPEAT_WC_4   (x)  
#define REPEAT_WC_16(x)   REPEAT_WC_8   (x), REPEAT_WC_8   (x)
#define REPEAT_WC_32(x)   REPEAT_WC_16  (x), REPEAT_WC_16  (x)
#define REPEAT_WC_64(x)   REPEAT_WC_32  (x), REPEAT_WC_32  (x)
#define REPEAT_WC_128(x)  REPEAT_WC_64  (x), REPEAT_WC_64  (x)  
#define REPEAT_WC_256(x)  REPEAT_WC_128 (x), REPEAT_WC_128 (x)
#define REPEAT_WC_512(x)  REPEAT_WC_256 (x), REPEAT_WC_256 (x)
#define REPEAT_WC_1024(x) REPEAT_WC_512 (x), REPEAT_WC_512 (x)


//=================| #REPEAT_WC (count, text) |=================
// Textually repeat 'what' 'cnt' times, delimited by comas.
//
// Example: int ones[16] = { REPEAT_WC (16, 1) };
//________________________________________________________rado__
#define REPEAT_WC(count, text)   REPEAT_WC_##count (text)


//====================| #UNIQUE_NAME |==========================
// Expands to unique identifier on each line.
//
// Example: int UNIQUE_NAME = 0,
//              UNIQUE_NAME = 1;
//________________________________________________________rado__
#define UNIQUE_NAME_1(prefix, x)   prefix##x
#define UNIQUE_NAME_2(prefix, x)   UNIQUE_NAME_1  (prefix, x)

#define UNIQUE_NAME_WP(prefix)     UNIQUE_NAME_2  (prefix, __LINE__)

#define UNIQUE_NAME                UNIQUE_NAME_WP (uniqueNameOnLine_)


//====================| #LINE_STRING |==========================
// __LINE__ as a string
//
// Example: printf (LINE_STRING);
//________________________________________________________rado__
#define LINE_STRING_1(x)   #x
#define LINE_STRING_2(x)   LINE_STRING_1 (x)

#define LINE_STRING        LINE_STRING_2 (__LINE__)


//=====================| #HERE |================================
// File name and line number in string form like "File (1234):"
//
// Example (MSVC): 
//     #pragma message (HERE "Click here!")
//________________________________________________________rado__
#define HERE  __FILE__ "("  LINE_STRING "):"


//===================| #ONCE(execution_code) |==================
// Executes 'execution_code' only the first time.
//
// Example: ONCE (cout << "Here we start"; )
//________________________________________________________rado__
#define ONCE(execution_code)      \
{                                 \
  static int UNIQUE_NAME = 0;     \
  if (!UNIQUE_NAME)               \
  {                               \
    UNIQUE_NAME = 1;              \
    { execution_code }            \
  }                               \
}


//=====================| #SKIP(cnt,code) |======================
// Executes 'execution_code' once in 'skip_count' times.
//
// Example: SKIP (100, cout << '*' << endl; }
//________________________________________________________rado__
#define SKIP(skip_count, execution_code)   \
{                                          \
  static int UNIQUE_NAME = 0;              \
  if (++UNIQUE_NAME >= (skip_count))       \
  {                                        \
    UNIQUE_NAME = 0;                       \
    { execution_code }                     \
  }                                        \
}


//=====================| #REV_SKIP(cnt,code) |==================
// Executes 'execution_code' once in 'skip_count' times.
//
// Example: REF_SKIP (100, cout << '*' << endl; }
//________________________________________________________rado__
#define REV_SKIP(skip_count, execution_code) \
{                                            \
  static int UNIQUE_NAME = 0;                \
  if (++UNIQUE_NAME >= (skip_count))         \
  {                                          \
    UNIQUE_NAME = 0;                         \
  }                                          \
  else                                       \
  {                                          \
    execution_code                           \
  }                                          \
}


//=====================| #LOOP_C |==============================
// Execute 'loop_code' 'count' times
//
// Example: LOOP_C (100, cout << '*' << endl; )
//________________________________________________________rado__
#define LOOP_C(count, loop_code)             \
{                                            \
  int UNIQUE_NAME = (count);                 \
  for (; UNIQUE_NAME > 0; --UNIQUE_NAME)     \
  { loop_code }                              \
}


//////////// C++ compatible only templates and macros //////////

#if CPP_MODE_COMPILATION

//=====================| #LOOP |================================
// 'for' loop header for loop to be executed 'count' times.
//  The statement, following it, is the loop body.
//
//  Example: LOOP (rand() % 100)
//               cout << '*';
//________________________________________________________rado__
#define LOOP(count)    for (int UNIQUE_NAME = (count);    \
                                UNIQUE_NAME > 0;          \
                                --UNIQUE_NAME)                 
// loop body here


//========================| #AT_START |=========================
// Execute 'execution_code' upon globals initialization.
//
// Example:
//     AT_START (cout << "executed before main()!\n")
//________________________________________________________rado__
#define AT_START(execution_code)      \
                                      \
  static struct UNIQUE_NAME           \
  {                                   \
    UNIQUE_NAME()                     \
    {                                 \
      execution_code                  \
    }                                 \
  }                                   \
  UNIQUE_NAME_WP (atStartVarOnLine_);                       


//=======================| #AT_END |============================
// Execute 'execution_code' upon globals cleanup.
//
// Example:
//     AT_END (cout << "executed AFTER main() return!\n")
//________________________________________________________rado__
#define AT_END(execution_code)                  \
                                                \
   static struct UNIQUE_NAME                    \
            {                                   \
            ~UNIQUE_NAME()                      \
               {                                \
               execution_code                   \
               }                                \
            }                                   \
      UNIQUE_NAME_WP (atStartVarOnLine_);  


//*************| class DelayedAssigner_T_Base |*****************
// Base virtual class for DelayedAssigner_T<>
// Example: see DelayedAssigner_T<>
//________________________________________________________rado__
class DelayedAssigner_T_Base
{
public:
  virtual ~DelayedAssigner_T_Base()
  {}
};


//***************| class DelayedAssigner_T |********************
// Initialise 'itsVarRef' and 'itsValue' on construction; 
// assign 'itsValue' to 'itsVarRef' upon destruction.
//
// Example: DelayedAssigner_T<int> saved_var (var);
//________________________________________________________rado__
template <class Type>
class DelayedAssigner_T : public DelayedAssigner_T_Base
{
  Type& itsVarRef;    // Reference to be assigned to
  Type  itsValue;     // Value to be remembered and assigned
     
public:
  DelayedAssigner_T (Type &var_ref) 
    : itsVarRef (var_ref)
    , itsValue  (var_ref)
  {}

  DelayedAssigner_T (Type &var_ref, const Type &value) // separate
    : itsVarRef (var_ref)
    , itsValue  (value)
  {}

  ~DelayedAssigner_T()
  {
    itsVarRef = itsValue;  
  }
};


//==================| #DELAYED_ASSIGN_T |=======================
// Assign 'value' to 'var_ref' upon destruction.
//
// Example:  DELAYED_ASSIGN_T (Node*, n, n->next)
//________________________________________________________rado__
#define DELAYED_ASSIGN_T(Type, var_ref, value)            \
                                                          \
   DelayedAssigner_T<Type> UNIQUE_NAME (var_ref, value);


//===================| #SAVE_T |================================
// Restore initial value of 'var_ref' upon destruction.
//
// Example:  SAVE_T (double, d)
//________________________________________________________rado__
#define SAVE_T(Type, var_ref)                             \
                                                          \
      DelayedAssigner_T<Type> UNIQUE_NAME (var_ref);


//===============| makeDelayedAssigner_T |======================
// Return DelayedAssigner_T <Type> (deducing 'Type' from 
// arguments)
//
// Example: see #define SAVE  
//________________________________________________________rado__
template <class Type> inline 
DelayedAssigner_T<Type> makeDelayedAssigner_T (Type &var)
{
  return DelayedAssigner_T<Type> (var);
} 

//===============| makeDelayedAssigner_T(2) |===================
// Returns DelayedAssigner_T <Type> (deducing 'Type' from 
// arguments)
//
// Example: see #define DELAYED_ASSIGN
//________________________________________________________rado__
template <class Type> inline 
DelayedAssigner_T <Type>
makeDelayedAssigner_T (Type &var, const Type& value)
{
  return DelayedAssigner_T<Type> (var, value);
} 


//==================| newDelayedAssigner_T |====================
// Allocate, initialize, and return pointer to a 
// DelayedAssigner_T <Type> (deducing 'Type' from argument)
//
// Example: std::auto_ptr <DeleyedAssigner_T_Base>              
//             save_var (allocateDelayedAssigner_T (j);
//________________________________________________________rado__
template <class Type> inline 
DelayedAssigner_T_Base* newDelayedAssigner_T (Type &var)
{
  return new DelayedAssigner_T<Type> (var);
} 


//======================| UnivesalStorage_T |===================
// Class that enables virtual storage, construction and 
// destruction.
//
// Example:
//    UnivesalStorage_T <BaseClass, sizeof (function())> 
//       auto_var (function());
//________________________________________________________rado__
template <class Base, size_t SIZE>
class UnivesalStorage_T
{
  char itsStorage [SIZE];  // binary storage
public:

  template <class From>   
  UnivesalStorage_T (const From& from)
  {
    // Static checks for mismatched parameter type:
    typedef char assertSize [sizeof (from) == SIZE];
    // will not compile if sizeof 'From' is wrong
    if (const Base* assertInheritence = &from)
      0;// will not compile if 'From' is not : public Base 

    new (itsStorage) From (from); // placement new & copy ctor
  }

  ~UnivesalStorage_T()
  {
    ((Base*)itsStorage)->~Base(); // hopefully virtual
  }
};


//=====================| #SAVE(var_ref) |=======================
// Restore initial value of 'var_ref' upon destruction.
//
// Example: SAVE (k)
//________________________________________________________rado__
#define SAVE(var_ref)                                          \
                                                               \
   UnivesalStorage_T                                           \
         < DelayedAssigner_T_Base,                             \
           sizeof (makeDelayedAssigner_T (var_ref)) >          \
      UNIQUE_NAME (makeDelayedAssigner_T (var_ref));


//======================| #DELAYED_ASSIGN |======================
// Assign 'value' to 'var_ref' upon destruction.
//
// Example: DELAYED_ASSIGN (n, n->next)
//________________________________________________________rado__
#define DELAYED_ASSIGN(var_ref, value)                         \
                                                               \
   UnivesalStorage_T                                           \
         < DelayedAssigner_T_Base,                             \
           sizeof (makeDelayedAssigner_T (var_ref)) >          \
      UNIQUE_NAME (makeDelayedAssigner_T ((var_ref), (value)));



#endif // CPP_MODE_COMPILATION

// ---------------------- end of file ------------------------
