`timescale 1ns/10ps
module CS(Y, X, reset, clk);

  input clk, reset; 
  input [7:0] X;
  output [9:0] Y;
  
  //--------------------------------------
  //  \^o^/   Write your code here~  \^o^/
  //--------------------------------------

  reg [7:0] x1,x2,x3,x4,x5,x6,x7,x8,x9,diff;
  reg [3:0] count;
  reg [9:0] Y, temp;

  always @(negedge reset or posedge clk) begin
    if(reset)
      begin
        count = 32'd0;
      end
    else
      if(count < 32'd8)
      begin
        case(count)
          32'd0:begin x1 = X; end
          32'd1:begin x2 = X; end
          32'd2:begin x3 = X; end
          32'd3:begin x4 = X; end
          32'd4:begin x5 = X; end
          32'd5:begin x6 = X; end
          32'd6:begin x7 = X; end
          default:begin x8 = X; end
        endcase
        count = count + 32'd1;
      end
      else
      begin
        if(count == 32'd8)
          begin
            x9 = X; 
            count = count + 32'd1;  
          end
        else
          begin 
            x1 = x2;
            x2 = x3;
            x3 = x4;
            x4 = x5;
            x5 = x6;
            x6 = x7;
            x7 = x8;
            x8 = x9;
            x9 = X;
          end

        temp = (x1 + x2 + x3 + x4 + x5 + x6 + x7 + x8 + x9) / 9;
        diff = 32'd0;
        if((temp >= x1) && ((temp - x1) < (temp - diff))) begin diff = x1; end else begin diff = diff; end
        if((temp >= x2) && ((temp - x2) < (temp - diff))) begin diff = x2; end else begin diff = diff; end
        if((temp >= x3) && ((temp - x3) < (temp - diff))) begin diff = x3; end else begin diff = diff; end
        if((temp >= x4) && ((temp - x4) < (temp - diff))) begin diff = x4; end else begin diff = diff; end
        if((temp >= x5) && ((temp - x5) < (temp - diff))) begin diff = x5; end else begin diff = diff; end
        if((temp >= x6) && ((temp - x6) < (temp - diff))) begin diff = x6; end else begin diff = diff; end
        if((temp >= x7) && ((temp - x7) < (temp - diff))) begin diff = x7; end else begin diff = diff; end
        if((temp >= x8) && ((temp - x8) < (temp - diff))) begin diff = x8; end else begin diff = diff; end
        if((temp >= x9) && ((temp - x9) < (temp - diff))) begin diff = x9; end else begin diff = diff; end

        Y = ((diff + x1) + (diff + x2) + (diff + x3) + (diff + x4) + (diff + x5) + (diff + x6) + (diff + x7) + (diff + x8) + (diff + x9)) / 8;
      end
  end

endmodule