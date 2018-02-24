# Python Version: 3.x
import sys
import re
import urllib.parse
import json
import os
import requests
import bs4
import onlinejudge.implementation.utils as utils
import onlinejudge.implementation.logging as log


# Copy from implementation/main.py
default_data_dir = os.path.join(os.environ.get('XDG_DATA_HOME') or os.path.expanduser('~/.local/share'), 'onlinejudge')
default_cookie_path = os.path.join(default_data_dir, 'cookie.jar')

with utils.with_cookiejar(utils.new_default_session(), path = default_cookie_path) as session:
    session = session or requests.Session()
    url = sys.argv[1]
    # get
    log.status('GET: %s', url)
    resp = session.get(url)
    log.status(utils.describe_status_code(resp.status_code))
    resp.raise_for_status()

    # parse
    soup = bs4.BeautifulSoup(resp.content.decode(resp.encoding), utils.html_parser)

    print(soup)
