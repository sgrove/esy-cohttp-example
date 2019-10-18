# cohttp-request-demo


[![CircleCI](https://circleci.com/gh/sgrove/cohttp-request-demo/tree/master.svg?style=svg)](https://circleci.com/gh/sgrove/cohttp-request-demo/tree/master)


**Contains the following libraries and executables:**

```
cohttp-request-demo@0.0.0
│
├─test/
│   name:    TestCohttpRequestDemo.exe
│   main:    TestCohttpRequestDemo
│   require: cohttp-request-demo.lib
│
├─library/
│   library name: cohttp-request-demo.lib
│   namespace:    CohttpRequestDemo
│   require:
│
└─executable/
    name:    CohttpRequestDemoApp.exe
    main:    CohttpRequestDemoApp
    require: cohttp-request-demo.lib
```

## Developing:

```
npm install -g esy
git clone https://github.com/sgrove/esy-cohttp-example.git
esy install
esy build
```

## Running Binary:

After building the project, you can run the main binary that is produced.

```
esy x CohttpRequestDemoApp.exe 
```

## Running Tests:

```
# Runs the "test" command in `package.json`.
esy test
```
