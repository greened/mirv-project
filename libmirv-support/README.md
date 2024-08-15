# libmirv-support - A C++ library

The `libmirv-support` C++ library provides <SUMMARY-OF-FUNCTIONALITY>.


## Usage

To start using `libmirv-support` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libmirv-support ^<VERSION>
```

Then import the library in your `buildfile`:

```
import libs = libmirv-support%lib{<TARGET>}
```


## Importable targets

This package provides the following importable targets:

```
lib{<TARGET>}
```

<DESCRIPTION-OF-IMPORTABLE-TARGETS>


## Configuration variables

This package provides the following configuration variables:

```
[bool] config.libmirv_support.<VARIABLE> ?= false
```

<DESCRIPTION-OF-CONFIG-VARIABLES>
