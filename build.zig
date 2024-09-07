const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "main",
        .target = target,
        .optimize = optimize,
    });
    exe.linkSystemLibrary("c");

    const flags = .{ "-Wall", "-Wextra" };
    const sources: [2][]const u8 = .{
        "src/main.c",
        "src/lexer.c",
    };

    for (sources) |source| {
        exe.addCSourceFile(.{
            .file = .{ .path = source },
            .flags = &flags,
        });
    }

    b.installArtifact(exe);
    const run_exe = b.addRunArtifact(exe);
    run_exe.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_exe.addArgs(args);
    }

    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&run_exe.step);
    b.default_step.dependOn(&exe.step);
}
