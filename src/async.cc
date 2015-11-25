class Async : public Nan::AsyncWorker {
 public:
  Async(Nan::Callback *callback, WorkerData data, int concurency)
    : AsyncWorker(callback), data(data), concurency(concurency) {}
  ~Async() {}
 private:
  Local<Array> obj = Nan::New<Array>();
  WorkerData data;
  int concurency;

  // Executed inside the worker-thread.
  // It is not safe to access V8, or V8 data structures
  // here, so everything we need for input and output
  // should go on `this`.
  void Execute () {
    cluster(data, concurency);
  }

  // Executed when the async work is complete
  // this function will be run inside the main event loop
  // so it is safe to use V8 again
  void HandleOKCallback () {
    Nan::HandleScope scope;

    Local<Array> obj = Nan::New<Array>();
    int i = 0;
    for( i=0; i < results.size(); i++ ){
        obj->Set(i, Nan::New<String>(results[i].c_str()).ToLocalChecked() );
    }

    Local<Value> argv[] = {
        Null()
      , obj
    };

    callback->Call(2, argv);
  }

 private:
  int points;
  double estimate;
};