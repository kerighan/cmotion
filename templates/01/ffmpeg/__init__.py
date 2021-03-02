import subprocess
import os


def frames_to_video(in_folder, out_file, frame_rate=24, crf=23):
    file_format = "jpg"
    preset = "medium"
    tune = None

    if tune is None:
        cmd = (
            """ffmpeg -y -loglevel panic -r {fr} -i {ifl}/%6d.{format} """
            """-c:v libx264 """
            # """-color_trc smpte2084 -color_primaries bt2020 """
            # """-x264-params "keyint=15:no-deblock=1" """
            # """-pix_fmt yuv420p -sws_flags spline+accurate_rnd+full_chroma_int """
            """-pix_fmt yuv420p """
            """-vf "colormatrix=bt470bg:bt709,fps=fps={fr}" """
            """-color_range 1 -colorspace 1 -color_primaries 1 -color_trc 1 """
            """-force_key_frames expr:gte\\(t,n_forced*1\\) """
            """-crf {crf} {of}""")
    else:
        cmd = (
            """ffmpeg -y -loglevel panic -r {fr} -i {ifl}/%6d.{format} """
            """-c:v libx264 -tune {tune} -vf "fps=fps={fr}" """
            """-force_key_frames expr:gte\\(t,n_forced*1\\) """
            """-crf {crf} -maxrate 2M -bufsize 1M -pix_fmt rgb24 {of}""")

    cmd = cmd.format(of=out_file,
                     fr=frame_rate,
                     ifl=in_folder,
                     format=file_format,
                     pr=preset,
                     tune=tune,
                     crf=crf)
    subprocess.call(cmd, shell=True)


def video_to_frames(
    in_file,
    out_folder,
    qscale=10,
    file_format="jpg",
    frame_rate=24,
    timestamps=(0, -1),
    video_size=None,
    crop=None
):
    if not os.path.exists(out_folder):
        os.makedirs(out_folder)
    else:
        for filename in os.listdir(out_folder):
            os.remove(os.path.join(out_folder, filename))

    if video_size is not None:
        video_size = ":".join([str(s) for s in video_size])

    command = []
    # input file
    command.append(f'ffmpeg -y -loglevel panic -i "{in_file}"')
    # alter quality
    command.append(f"-qscale:v {qscale}")
    # frame rate
    command.append(f"-r {frame_rate}")
    # change start and end time
    # if start != 0 or end != -1:

    command.append(f'"{out_folder}/%d.{file_format}"')
    command = " ".join(command)

    # if timestamps is not None:
    #     start, end = timestamps
    #     if video_size is not None:
    #         cmd = (
    #             f"""ffmpeg -y -loglevel panic -i "{in_file}" """
    #             f"""-vf scale={video_size} """
    #             f"""-qscale:v {qscale} """
    #             f"""-ss {start} -to {end} -r {frame_rate} """
    #             f""""{out_folder}/%d.{file_format}" """)
    #     elif crop is None:
    #         cmd = (
    #             f"""ffmpeg -y -loglevel panic -i "{in_file}" """
    #             f"""-qscale:v {qscale} """
    #             f"""-ss {start} -to {end} -r {frame_rate} """
    #             f""""{out_folder}/%d.{file_format}" """)
    #     else:
    #         out_w = crop[2] - crop[0]
    #         out_h = crop[3] - crop[1]
    #         x, y = crop[0], crop[1]
    #         cmd = (
    #             f"""ffmpeg -y -i "{in_file}" """
    #             f"""-filter:v "crop={out_w}:{out_h}:{x}:{y}" """
    #             f"""-qscale:v {qscale} """
    #             f"""-ss {start} -to {end} -r {frame_rate} """
    #             f""""{out_folder}/%d.{file_format}" """)
    # else:
    #     if video_size is not None:
    #         cmd = (
    #             f"""ffmpeg -y -loglevel panic -i "{in_file}" """
    #             f"""-vf scale={video_size} """
    #             f"""-qscale:v {qscale} """
    #             f"""-r {frame_rate} """
    #             f""""{out_folder}/%d.{file_format}" """)
    #     elif crop is None:
    #         cmd = (
    #             f"""ffmpeg -y -loglevel panic -i "{in_file}" """
    #             f"""-qscale:v {qscale} """
    #             f"""-r {frame_rate} """
    #             f""""{out_folder}/%d.{file_format}" """)
    #     else:
    #         out_w = crop[2] - crop[0]
    #         out_h = crop[3] - crop[1]
    #         x, y = crop[0], crop[1]
    #         cmd = (
    #             f"""ffmpeg -y -loglevel panic -i "{in_file}" """
    #             f"""-filter:v "crop={out_w}:{out_h}:{x}:{y}" """
    #             f"""-qscale:v {qscale} """
    #             f"""-r {frame_rate} """
    #             f""""{out_folder}/%d.{file_format}" """)
    subprocess.call(command, shell=True)
