# Sections

## SECTION(*name* [, *qualifier*])

Defines a section within a test case. Depending on the qualifier, it will
be evaluated in different scenarios.

### Arguments

- *name*: A free-form string literal. Must be unique within this nesting
  level of the parent section/test case.
- *qualifier*: Specifies when the secion is evaluated. If no qualifier is
  specified, *both* is assumed. Must be one of:
    + *runtime*: This section is only evaluated at runtime.
    + *compiletime*: This section is only evaluated at compiletime.
    + *both*: This is evaluated at runtime and compiletime.
    + *runtime_if(expr)*: If *expr* evaluates to true, the section behaves
      as if *runtime*-qualified. Otherwise, it behaves as if *compiletime*-
      qualified.
    + *compiletime_if(expr)*: If *expr* evaluates to true, the section
      behaves as if *compiletime*-qualified. Otherwise, it behaves as if
      *runtime*-qualified.
    + *only_if(expr)*: If *expr* evaluates to true, the section behaves
      as if *both*-qualified. Otherwise, it behaves as if
      *ignore*-qualified.
    + *only_runtime_if(expr)*: If *expr* evaluates to true, the section
      behaves as if *runtime*-qualified. Otherwise, it behaves as if
      *ignore*-qualified.
    + *only_compiletime_if(expr)*: If *expr* evaluates to true, the section
      behaves as if *compiletime*-qualified. Otherwise, it behaves as if
      *ignore*-qualified.
    + *disabled*: This section is never evaluated.

## Examples

```c++
SECTION("comparison with itself must return true") {}
SECTION("check operator<<(std::ostream&) output", runtime) {}
```

## DYNAMIC_SECTION(*name* [, *qualifier*])

Defines a section with a dynamic name within a test case. Depending on the
qualifier, it will be evaluated in different scenarios.

### Arguments

- *name*: A string of type `bs::structural_string`. Must be unique within
  this nesting level of the parent section/test case.
- *qualifier*: Specifies when the secion is evaluated. If no qualifier is
  specified, *both* is assumed. Must be one of:
    + *runtime*: This section is only evaluated at runtime.
    + *compiletime*: This section is only evaluated at compiletime.
    + *both*: This is evaluated at runtime and compiletime.
    + *runtime_if(expr)*: If *expr* evaluates to true, the section behaves
      as if *runtime*-qualified. Otherwise, it behaves as if *compiletime*-
      qualified.
    + *compiletime_if(expr)*: If *expr* evaluates to true, the section
      behaves as if *compiletime*-qualified. Otherwise, it behaves as if
      *runtime*-qualified.
    + *only_if(expr)*: If *expr* evaluates to true, the section behaves
      as if *both*-qualified. Otherwise, it behaves as if
      *ignore*-qualified.
    + *only_runtime_if(expr)*: If *expr* evaluates to true, the section
      behaves as if *runtime*-qualified. Otherwise, it behaves as if
      *ignore*-qualified.
    + *only_compiletime_if(expr)*: If *expr* evaluates to true, the section
      behaves as if *compiletime*-qualified. Otherwise, it behaves as if
      *ignore*-qualified.
    + *disabled*: This section is never evaluated.

## Examples

```c++
DYNAMIC_SECTION("Still a literal") {}
DYNAMIC_SECTION("test " + bs::stringify_typename<Type>(), runtime_if(!std::is_trivially_constructible_v<Type>)) {}
```