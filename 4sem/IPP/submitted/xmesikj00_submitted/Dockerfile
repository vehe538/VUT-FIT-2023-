FROM python:3.14 AS check
RUN apt-get update && apt-get install -y curl xz-utils
ENV NODE_VERSION=25.2.0
RUN curl -fsSL https://nodejs.org/dist/v$NODE_VERSION/node-v$NODE_VERSION-linux-x64.tar.xz \
    | tar -xJ -C /usr/local --strip-components=1

COPY int/requirements-dev.txt .
COPY int/requirements.txt .
COPY tester/package.json .
RUN pip install -r requirements-dev.txt -r requirements.txt
RUN npm install
ENTRYPOINT ["bash"]


FROM node:25.2 AS build-test
WORKDIR /tester/
COPY tester/ .
RUN npm install
RUN npx tsc --project tsconfig.json


FROM python:3.14-slim AS runtime
COPY int/ interpreter/
RUN pip install -r interpreter/requirements.txt
WORKDIR interpreter/
ENTRYPOINT ["python", "src/solint.py"]


FROM runtime AS test
RUN apt-get update && apt-get install -y nodejs gcc libxml2-dev libxslt-dev
COPY --from=build-test /tester/dist/ /tester/dist/
COPY --from=build-test /tester/node_modules/ /tester/node_modules/
COPY /tester/sol2xml/ /tester/sol2xml/
RUN pip install lxml lark
WORKDIR /tester/
ENTRYPOINT ["node", "dist/tester.js"]
