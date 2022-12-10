module SME(clk,reset,chardata,isstring,ispattern,valid,match,match_index);
input clk;
input reset;
input [7:0] chardata;
input isstring;
input ispattern;
output match;
output [4:0] match_index;
output valid;
reg match;
reg [4:0] match_index;
reg valid;

reg [7:0] words[31:0], pattern[7:0];
reg [8:0] strCount, patCount;
reg start, last, cmpChk;
integer i, j;

//save input
always @(posedge clk or posedge reset) begin
  if(reset) begin
    for(i = 0 ; i < 32 ; i=i+1) begin
      words[i] = 8'd0;
      if(i < 32'd8)
        pattern[i] = 8'd0;
    end
    match = 0;
    match_index = 0;
    start = 0;
    last = 0;
    strCount = 0;
    patCount = 0;
    cmpChk = 0;
    valid = 0;
  end
  else begin
    if(isstring) begin
      valid = 0;
      if(cmpChk) begin
        strCount = 0;
        cmpChk = 0;
      end
      words[strCount] <= chardata;
      strCount <= strCount + 1;
    end
    else if(ispattern) begin
      valid = 0;
      if(chardata == 8'h5E)
        start = 1;
      else if(chardata == 8'h24)
        last = 1;
      else begin
        pattern[patCount] = chardata;
        patCount <= patCount + 1;
      end
    end
    else begin
      strCount <= strCount;
      patCount <= patCount;
    end
  end
end

//pattern and string match
always @(ispattern) begin
  if(!ispattern) begin
    cmpChk = 1;
    j = 0;
    match = 0;
    match_index = 0;

    for(i = 0 ; i < strCount ; i = i + 1) begin
      if((pattern[j] == words[i]) || (pattern[j] == 8'h2E)) begin
        if(j == 0)
          match_index = i;
        
        j = j + 1;
      end
      else begin
        i = i - j; //push back
        j = 0;
      end

      if(j == patCount) begin
        if(start) begin
          if((words[match_index - 1] != 8'h20) && (match_index != 0) ) begin 
            i = i - j + 1;
            j = 0;
          end
          else begin
            j = j;
          end
        end
        else begin
          j = j;
        end
          
        if(last && (j > 0)) begin
          if(((i + 1) < strCount) && (words[i + 1] != 8'h20)) begin
            i = i - j + 1;
            j = 0;
          end
          else begin
            j = j;
          end
        end
        else begin
          j = j;
        end

        if(j != 0) begin
          i = strCount;
          match = 1;
          valid = 1;
        end
        else begin
          j = j;
        end
      end
    end
  end

  if(j != patCount) begin
    match = 0;
    valid = 1;
  end
end


//checking the match
always @(valid) begin
  if(valid) begin
    start = 0;
    last = 0;
    patCount = 0;
  end
end

endmodule