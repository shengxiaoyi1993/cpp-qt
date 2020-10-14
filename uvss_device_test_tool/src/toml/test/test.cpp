#include <sys/time.h>
#include "../src/toml.h"

int main(int argc, char **argv) {
  Toml *d = new Toml(argv[1]);

  string region;

  cout << d->Get("region", region) << endl;

  cout << "region:" << region << endl;

  bool h;

  d->Get("hello", h);

  cout << "hello:" << h << endl;

  d->Set("me", "jry");

  d->Set("valid", true);

  d->Set("valid2", false);

  d->Set("grade", 1.234);

  d->Set("num", 12);

  cout << "===============" << endl;

  string dtr;
  d->Get("me", dtr);

  cout << "dtr:" << dtr << endl;

  cout << d->toString() << endl;

  time_t tnow;
  time(&tnow);

  d->Set("last_count_time", &tnow);

  d->Save("hello.toml");
}
