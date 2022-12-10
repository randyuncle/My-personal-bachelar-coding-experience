module traffic_light (
    input  clk,
    input  rst,
    input  pass,
    output reg R,
    output reg G,
    output reg Y
);

//write your code here
//reg R, G, Y;
reg [32:0] count;
reg [2:0] state, nextState, noneCount;
parameter cycleG = 32'd512, cycleR = 32'd512, cycleY = 32'd256, cycleGl = 32'd64, cycleN = 32'd64;
parameter stateG = 2'd0, stateN = 2'd1, stateY = 2'd2, stateR = 2'd3;

//state change and count the cycle
always@(posedge clk or posedge rst)begin
    if(rst) begin
        count = 32'd0;
        noneCount = 32'd0;
        state = stateG;
        nextState = stateN;
    end
    else if(pass && (state != stateG)) begin
        count = 32'd0;
        noneCount = 32'd0;
        state = stateG;
        nextState = stateN;
    end
    else begin
        count = count + 1;
        case(state)
        stateG: begin
            if(noneCount == 2'd0) begin
                if(count == cycleG) begin
                    count = 32'd0;
                    noneCount = noneCount + 1;
                    state = nextState;
                    nextState = stateG;
                end
                else
                    state = state;
            end
            else begin
                if(count == cycleGl) begin
                    count = 32'd0;
                    if(noneCount == 2'd1) begin
                        noneCount = noneCount + 1;
                        state = nextState;
                        nextState = stateG;
                    end
                    else begin
                        noneCount = 2'd0;
                        state = nextState;
                        nextState = stateR;
                    end
                end
                else
                    state = state;
            end
        end
        stateN:begin
            if(count == cycleN)begin
                count = 32'd0;
                state = nextState;
                if(noneCount == 2'd1)
                    nextState = stateN;
                else
                    nextState = stateY;
            end
            else
                state = state;
        end
        stateY:begin
            if(count == cycleY)begin
                count = 32'd0;
                state = nextState;
                nextState = stateG;
            end
            else
                state = state;
        end
        stateR:begin
            if(count == cycleR)begin
                count = 32'd0;
                state = nextState;
                nextState = stateN;
            end
            else
                state = state;
        end
        endcase
    end
end

//light control
always @(state) begin
    case(state)
    stateG: begin
        G = 1;
        Y = 0;
        R = 0;
    end
    stateN: begin
        G = 0;
        Y = 0;
        R = 0;
    end
    stateY: begin
        G = 0;
        Y = 1;
        R = 0;
    end
    stateR:begin
        G = 0;
        Y = 0;
        R = 1;
    end
    endcase
end


endmodule
