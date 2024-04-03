import pytest

from testsuite.databases import pgsql


# Start the tests via `make test-debug` or `make test-release`

@pytest.mark.parametrize(
    'request_body, expected_resp_code, expected_resp_body',
    [(
            {'first': [1, 2], 'second': [1, 2], 'thirst': [1, 2]},
            200,
            {'first': [1, 2], 'second': [1, 2], 'thirst': [1, 2, 3]}
    )]
)
async def test_req_validation(service_client, request_body, expected_resp_code, expected_resp_body):
    response = await service_client.post(
        '/handler',
        json=request_body,
        params={'index_add': 3, 'value_add': 3},
    )
    assert response.status == expected_resp_code
    assert response.json() == expected_resp_body


async def test_openapi(service_client):
    response = await service_client.get(
        '/openapi',
    )
    assert response.status == 200
    assert response.text == """info:
  title: Some Server Doc
  description: Some server
  version: 1.0.0
openapi: 3.0.0
servers:
  - description: stable
    url: top_secret
paths:
  /handler:
    post:
      description: Request
      requestBody:
        required: true
        content:
          application/json:
            schema:
              $ref: "#/components/schemas/Body"
      parameters:
        - in: query
          name: index_add
          required: false
          schema:
            type: integer
            format: int32
            minimum: 1
            maximum: 3
        - in: query
          name: value_add
          required: false
          schema:
            type: integer
            format: int64
      responses:
        200:
          $ref: "#/components/responses/Response"
components:
  schemas:
    Body:
      type: object
      properties:
        first:
          type: array
          minItems: 2
          items:
            type: integer
            format: int64
        second:
          type: array
          maxItems: 2
          items:
            type: integer
            format: int64
        thirst:
          type: array
          uniqueItems: true
          items:
            type: integer
            format: int64
  responses:
    Response:
      description: ""
      content:
        application/json:
          schema:
            $ref: "#/components/schemas/Body\""""
