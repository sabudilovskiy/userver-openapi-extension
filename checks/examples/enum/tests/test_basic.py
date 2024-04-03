import pytest

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`


async def test_operation(service_client):
    response = await service_client.post(
        '/operation',
        json={
            'left': 10,
            'right': 20
        },
        params={'op': 'prod'},
    )
    assert response.status == 200
    assert response.json() == {
        'result': 200
    }


async def test_openapi(service_client):
    response = await service_client.get(
        '/openapi',
    )
    assert response.status == 200
    assert response.text == """info:
  description: Some server
  title: Some Server Doc
  version: 1.0.0
openapi: 3.0.0
servers:
  - description: stable
    url: top_secret
paths:
  /operation:
    post:
      description: OperationRequest
      requestBody:
        required: true
        content:
          application/json:
            schema:
              $ref: "#/components/schemas/OperationBody"
      parameters:
        - in: query
          name: op
          required: true
          schema:
            $ref: "#/components/schemas/Operation"
      responses:
        200:
          $ref: "#/components/responses/Response"
components:
  schemas:
    OperationBody:
      type: object
      properties:
        left:
          type: integer
          format: int64
        right:
          type: integer
          format: int64
      required:
        - left
        - right
    Operation:
      type: string
      enum:
        - sum
        - div
        - sub
        - prod
    ResponseBody:
      type: object
      properties:
        result:
          type: integer
          format: int64
      required:
        - result
  responses:
    Response:
      description: ""
      content:
        application/json:
          schema:
            $ref: "#/components/schemas/ResponseBody\""""
