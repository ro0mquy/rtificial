#!/bin/bash
rsync -Przh ./www.rtificial.org/         rtificial.org:/var/www/virtual/rtfcl/html/
rsync -Przh ./hinterzimmer.rtificial.org rtificial.org:/var/www/virtual/rtfcl/
rsync -Przh ./zeuch.rtificial.org        rtificial.org:/var/www/virtual/rtfcl/
