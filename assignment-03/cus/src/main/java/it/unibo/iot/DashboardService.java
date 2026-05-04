package it.unibo.iot;

import io.vertx.core.AbstractVerticle;
import io.vertx.core.http.HttpMethod;
import io.vertx.core.http.HttpServerResponse;
import io.vertx.core.json.JsonObject;
import io.vertx.ext.web.Router;
import io.vertx.ext.web.handler.BodyHandler;
import io.vertx.ext.web.handler.CorsHandler;

public class DashboardService extends AbstractVerticle {

    @Override
    public void start() {
        Router router = Router.router(vertx);

        //Configurazione CORS per permettere richieste da qualsiasi origine
        router.route().handler(CorsHandler.create()
                .addOrigin("*")
                .allowedMethod(HttpMethod.GET)
                .allowedMethod(HttpMethod.POST)
                .allowedMethod(HttpMethod.OPTIONS)
                .allowedHeader("Content-Type"));

        router.route().handler(BodyHandler.create());

        // API Endpoint per i dati
        router.get("/api/data").handler(routingContext -> {
            HttpServerResponse response = routingContext.response();
            response.putHeader("content-type", "application/json");

            JsonObject json = new JsonObject()
                .put("level", CusMain.currentWaterLevel)
                .put("percentage", CusMain.currentWaterPercentage)
                .put("valve", String.valueOf(CusMain.currentValvePerc))
                .put("waterState", CusMain.waterState.toString())
                .put("netState", CusMain.netState.toString())
                .put("mode", CusMain.currentMode.toString());
            response.end(json.encode());
        });

        // API ricezione comandi
        router.post("/api/command").handler(routingContext -> {
            JsonObject body = routingContext.body().asJsonObject();
            String action = body.getString("action");

            if ("SET_MODE".equals(action)) {
                String mode = body.getString("mode");
                CusMain.currentMode = "MANUAL".equals(mode) ? CusMain.Mode.MANUAL : CusMain.Mode.AUTOMATIC;
                System.out.println("DBS-> Cambio Modalità in: " + CusMain.currentMode);
                
            } else if ("SET_VALVE".equals(action)) {
                if (CusMain.currentMode == CusMain.Mode.MANUAL) {
                    int val = body.getInteger("value");
                    CusMain.sendSerialCmd("VALVE:" + val);
                    System.out.println("DBS-> Valvola MANUALE a: " + val + "%");
                }
            }
            routingContext.response().end(new JsonObject().put("status", "ok").encode());
        });

        // Avvio server HTTP
        vertx.createHttpServer().requestHandler(router).listen(8080, http -> {
            if (http.succeeded()) {
                System.out.println("API Server avviato su http://localhost:8080/api/data");
            } else {
                System.out.println("Errore avvio API Server: " + http.cause());
            }
        });
    }
}