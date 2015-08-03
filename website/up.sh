#!/bin/bash
rsync -Prhh ./www.rtificial.org/         rtfcl@rtificial.org:/var/www/virtual/rtfcl/html/
rsync -Prhh ./hinterzimmer.rtificial.org rtfcl@rtificial.org:/var/www/virtual/rtfcl/
rsync -Prhh ./zeuch.rtificial.org        rtfcl@rtificial.org:/var/www/virtual/rtfcl/
