
struct uniform_buffer {
    float4x4 model;
    float4x4 view;
    float4x4 proj;
};
ConstantBuffer<uniform_buffer> ubo;

struct vs_input {
    float3 in_pos;
    float3 in_norm;
    float2 in_uv;
};

struct vs_output {
    float4 out_pos : SV_Position;
    float3 out_norm;
    float2 out_uv;
};

[shader("vertex")]
vs_output vert_main(vs_input input) {
    vs_output output;
    output.out_pos = mul(ubo.proj, mul(ubo.view, mul(ubo.model, float4(input.in_pos, 1.0))));
    output.out_norm = input.in_norm;
    output.out_uv = input.in_uv;
    return output;
}

[shader("fragment")]
float4 frag_main(vs_output in_vert) : SV_Target {
    float3 color = in_vert.out_norm;
    return float4(color, 1.0);
}
